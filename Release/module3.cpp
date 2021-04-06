#include"module3.h"

namespace module3
{
    // 命名空间成员变量

    Mat src_gray;                               //源图片，源灰度图片 
    Mat dst, detected_edges;                    //目标图片，已检测边缘图片
    int lowThreshold = 80;                      //低阈值   设置为0是的均为强边缘点，从而获得更完整的边缘图片
    int ratio1 = 3;                             //高低阈值之比
    int kernel_size = 3;                        //内核大小
    int highThreshold = lowThreshold * ratio1;
    int idx = 0;

    //增强图像 //进行二值化操作
    Mat enhancement(Mat src) {
        for (int i = 0; i < src.rows; i++) {
            for (int j = 0; j < src.cols; j++) {
                src.ptr<uchar>(i)[j] = (src.ptr<uchar>(i)[j] > 128 ? 255 : 0);
            }
        }
        return src;
    }

    //Canny算法，Canny阈值输入比例1:3
    Mat CannyThreshold(Mat& src)
    {
        //高斯滤波
        GaussianBlur(src_gray, detected_edges, Size(9, 9), 0, 0);
        //双边滤波
        //bilateralFilter(src_gray, detected_edges, 5, 200, 200, BORDER_REFLECT_101);
        // 运行Canny算子，模糊图像，寻找边缘，然后值保存在detected_edges中
        Canny(detected_edges, detected_edges, lowThreshold, highThreshold, kernel_size);

        // 使用 Canny算子输出边缘作为掩码显示原图像，填充dst图像（全黑）
        dst = Scalar::all(0);

        //使用函数 copyTo 标识被检测到的边缘部分
        src.copyTo(dst, detected_edges);
        return dst;
    }

    //对图片进行膨化和腐蚀处理，去除不需要的边缘和闭合间隙
    void dilate_erode(Mat& src, Mat& dst)
    {
        //返回指定形状和尺寸的内核矩阵。选择矩形的 3*3 内核，锚点默认为中心点(-1,-1)
        Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

        //膨化处理
        dilate(src, dst, element);

        //腐蚀处理
        erode(dst, dst, element);

        for (int i = 0; i < 10; i++)
        {
            //膨化处理
            dilate(dst, dst, element);

            //腐蚀处理
            erode(dst, dst, element);
        }
    }


    //确定目标图片中的目标二维码所在区域
    vector<Point> getRect(Mat& src, Mat& dst)
    {
        double maxarea = 0;             //最大轮廓面积
        int maxAreaIdx = 0;             //最大面积轮廓的下标
        vector<vector<Point>>contours;  //轮廓对象
        vector<Vec4i>hierarchy;         //图像的拓扑结构

        //寻找轮廓，返回在contours中
        findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        //寻找最大轮廓
        for (int i = 0; i < contours.size(); i++)
        {
            double tmparea = fabs(contourArea(contours[i]));
            if (tmparea > maxarea)
            {
                maxarea = tmparea;
                maxAreaIdx = i;
                continue;
            }
        }

        Mat res_dst = Mat::zeros(src.size(), CV_8UC3);

        //设置轮廓的颜色
        RNG rng(0);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

        //绘制轮廓
        drawContours(res_dst, contours, maxAreaIdx, color, 2, 8, hierarchy, 0, Point(0, 0));

        //imshow("res", res_dst);

        //用于存储拟合多边形的顶点信息
        vector<Point>vertex_point;
        //对轮廓进行拟合，拟合成多边形
        approxPolyDP(contours[maxAreaIdx], vertex_point, 50, 1);

        //输出拟合成的多边形的顶点信息
        //cout << vertex_point << endl;

        return vertex_point;
    }

    //透视映射，返回映射后的图片
    Mat get_Mappedim(Mat src, vector<Point> point)
    {
        Point2f src_corners[4];          //源顶点信息
        Point2f dst_corners[4];          //目标顶点信息

        bool x[4] = { false,false,false,false };
        bool y[4] = { false,false,false,false };

        //为point元素x大小排序
        float x_tmp1;
        float x_tmp2;
        int idx11;
        int idx12;
        if (point[0].x < point[1].x)
        {
            x_tmp1 = point[0].x;
            idx11 = 0;
            x_tmp2 = point[1].x;
            idx12 = 1;
        }
        else
        {
            x_tmp1 = point[1].x;
            idx11 = 1;
            x_tmp2 = point[0].x;
            idx12 = 0;
        }

        for (int i = 2; i < 4; i++)
        {
            if (point[i].x < x_tmp2)
            {
                if (point[i].x < x_tmp1)
                {
                    x_tmp2 = x_tmp1;
                    idx12 = idx11;
                    x_tmp1 = point[i].x;
                    idx11 = i;
                }
                else
                {
                    x_tmp2 = point[i].x;
                    idx12 = i;
                }
            }
        }
        x[idx11] = true;
        x[idx12] = true;
        //为point元素y大小排序
        float y_tmp1;
        float y_tmp2;
        int idx21;
        int idx22;
        if (point[0].y < point[1].y)
        {
            y_tmp1 = point[0].y;
            idx21 = 0;
            y_tmp2 = point[1].y;
            idx22 = 1;
        }
        else
        {
            y_tmp1 = point[1].y;
            idx21 = 1;
            y_tmp2 = point[0].y;
            idx22 = 0;
        }

        for (int i = 2; i < 4; i++)
        {
            if (point[i].y < y_tmp2)
            {
                if (point[i].y < y_tmp1)
                {
                    y_tmp2 = y_tmp1;
                    idx22 = idx21;
                    y_tmp1 = point[i].y;
                    idx21 = i;
                }
                else
                {
                    y_tmp2 = point[i].y;
                    idx22 = i;
                }
            }
        }
        y[idx21] = true;
        y[idx22] = true;
        //格式的转换
        for (int i = 0; i < 4; i++)
        {
            if (x[i] && y[i])
            {
                src_corners[0].x = point[i].x;
                src_corners[0].y = point[i].y;
            }
            else if (x[i] && !y[i])
            {
                src_corners[1].x = point[i].x;
                src_corners[1].y = point[i].y;
            }
            else if (!x[i] && !y[i])
            {
                src_corners[2].x = point[i].x;
                src_corners[2].y = point[i].y;
            }
            else
            {
                src_corners[3].x = point[i].x;
                src_corners[3].y = point[i].y;
            }
        }
        //设置目标区域
        dst_corners[0] = Point(0, 0);
        dst_corners[1] = Point(0, 832);
        dst_corners[2] = Point(832, 832);
        dst_corners[3] = Point(832, 0);

        //获取透视矩阵
        Mat warpmatrix = getPerspectiveTransform(src_corners, dst_corners);

        Mat res;

        //进行透视
        warpPerspective(src, res, warpmatrix, res.size(), INTER_LANCZOS4);

        //截取目标区域的信息
        Rect res_rect(0, 0, 832, 832);
        res = res(res_rect);

        return res;
    }

    //module3的接口
    vector<Mat> get_QRcode(vector<Mat>& src_ims)
    {
        //int i = 1;
        vector<Mat> src;
        for (auto src_im : src_ims)
        {
            //cout << i++ << endl;
            if (!src_im.data)
                exit(-1);

            //对图片大小进行调整
            resize(src_im, src_im, Size(832, 832), 0, 0, INTER_LANCZOS4);

            //imshow("src", src_im);
            //创建与src_im同类型的大小的矩阵（dst）
            dst.create(src_im.size(), src_im.type());

            //原图像转换为灰度图像
            cvtColor(src_im, src_gray, CV_BGR2GRAY);

            //获取边缘
            dst = CannyThreshold(src_im);

            //转换为灰度图
            cvtColor(dst, dst, CV_BGR2GRAY);

            //膨化与腐蚀
            //dilate_erode(dst, dst);

            //得到二维码区域顶点
            vector<Point>point = getRect(dst, dst);

            //获取透视后的二维码
            src_im = get_Mappedim(src_im, point);


            //对二维码大小进行调整
            //resize(src_im, src_im, Size(820, 820), 0, 0, INTER_CUBIC);
            cvtColor(src_im, src_im, CV_BGR2GRAY);
            src_im = enhancement(src_im);

            //将截取到的二维码添加到容器src中
            src.push_back(src_im);
        }

        return src;
    }
}