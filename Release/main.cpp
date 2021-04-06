#include"main.h"
#pragma GCC optimize(2)
using namespace std;
using namespace cv;

namespace enhance {

    //增强图像
    Mat enhancement(Mat src) {
        for (int i = 0; i < src.rows; i++) {
            for (int j = 0; j < src.cols; j++) {
                src.ptr<uchar>(i)[j] = (src.ptr<uchar>(i)[j] > 70 ? 255 : 0);
            }
        }
        return src;
    }
}

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
    // 命名空间成员函数 -- 定义

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

namespace encoding {

    //信息块（正方形）边长
    const int PIX_SIZE = 8;

    //每行/列的信息块数量
    const int DATA_LENGTH = 800 / PIX_SIZE;

    //每张图片的最大信息块数量
    const int MAX_SIZE = DATA_LENGTH * DATA_LENGTH - 8;

    //最大帧数
    const int FRAME_MAX = 15;


    //读文件+返回string
    string getFile() {

        //打开文件
        string rt;

        fstream file("e5.bin", ios::binary | ios::in);
        if (!file) {
            cout << "Open file failed!" << endl;
            return rt;
        }

        unsigned char ch;

        while (file.read((char*)&ch, sizeof(ch)) && rt.size() < (FRAME_MAX * MAX_SIZE) / 8)rt += ch;

        return rt;
    }

    //文件转化成二进制
    string file2Binary(string s) {
        string rt;
        int len = s.length();
        for (int i = 0; i < len; i++) {
            unsigned char ch = s[i];
            for (int j = 0; j < 8; j++) {
                if (ch % 2)rt += '1';
                else rt += '0';
                ch /= 2;
            }
        }
        return rt;
    }

    //待写入文件分块
    vector<string> split(string s) {
        vector<string> rt;
        int len = s.size();
        int head = 0, end = min(head + MAX_SIZE, len);
        while (head != len) {
            string tmp;
            for (int i = head; i < end; i++) {
                tmp.push_back(s[i]);
            }
            rt.push_back(tmp);
            head = end;
            end = min(head + MAX_SIZE, len);
        }
        return rt;
    }

    //将string内存的二进制信息存入Mat
    Mat string2Mat(string s, int num) {
        Mat img(DATA_LENGTH, DATA_LENGTH, CV_8UC1);

        //存入张数信息
        for (int i = 7; i >= 0 ; i--) {
            img.ptr<uchar>(0)[i] = (num % 2  ? 255 : 0);
            num /= 2;
        }

        //存入数据
        int cnt = 0, len = s.size();
        for (int i = 0; i < DATA_LENGTH; i++) {
            for (int j = 0; j < DATA_LENGTH; j++) {
                if (i == 0 && j < 8)continue;
                if (cnt >= len)img.ptr<uchar>(i)[j] = 0;
                else img.ptr<uchar>(i)[j] = (s[cnt++] == '1' ? 255 : 0);
            }
        }

        //放大图片
        Mat bigger_img(800, 800, CV_8UC1);


        for (int i = 0; i < 800; i++) {
            for (int j = 0; j < 800; j++) {
                int x = i / PIX_SIZE;
                int y = j / PIX_SIZE;
                bigger_img.ptr<uchar>(i)[j] = img.ptr<uchar>(x)[y];
            }
        }


        //添加轮廓
        Mat rt(1000, 1000, CV_8UC1);

        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++) {
                rt.ptr<uchar>(i)[j] = 255;
                if (i <= 916 && i >= 84 && j <= 916 && j >= 84) {
                    rt.ptr<uchar>(i)[j] = 0;
                }
                /*现在黑框只有4pixel*/
                if (i <= 912 && i >= 88 && j <= 912 && j >= 88) {
                    rt.ptr<uchar>(i)[j] = 255;
                }
            }
        }

        //往轮廓内添加信息
        for (int i = 100; i < 900; i++) {
            for (int j = 100; j < 900; j++) {
                rt.ptr<uchar>(i)[j] = bigger_img.ptr<uchar>(i - 100)[j - 100];
            }
        }

        return rt;
    }

    //批量处理
    vector<Mat> string2Mat_vector(vector<string> s) {
        int len = s.size();

        //限制
        len = min(len, FRAME_MAX);
        vector<Mat> rt;
        for (int i = 0; i < len; i++) {
            Mat tmp = string2Mat(s[i], i + 1);
            rt.push_back(tmp);
        }
        return rt;
    }
}

namespace picvideo
{
    void pic2video(vector<Mat>& src_ims);//将元组内各类图片转化为视频



    void video2pic(string videopath, vector<Mat>& src_ims);//将视频逐帧提取进入元组成为Mat类对象


    void videoshow(string path);//用于播放视频内容，参数为当目录下文件名字


    void pic2video(vector<Mat>& src_ims)
    {
        string name2="E:\\test.avi";
        cout << "请输入编码后的视频文件名，例如“test.mp4”:" << endl;
        //cin >> name2;

        cout << "请输入视频帧率:" << endl;
        int frame_rate = 15;
        //cin >> frame_rate;

        VideoWriter video(name2, VideoWriter::fourcc('M', 'P', '4', 'V'), frame_rate, Size(1000, 1000),false);
        for (size_t i = 0; i < src_ims.size(); i++)
        {
            Mat image = src_ims[i].clone();
            // 流操作符，把图片传入视频
            video << image;
        }

        Mat img = src_ims[src_ims.size() - 1];
        int t = 30;
        while (t--)video << img;
        
    }

    void video2pic(string path, vector<Mat>& src_ims)
    {
        VideoCapture capture(path);
        Mat frame;
        while (true) {
            //一帧一帧读
            capture >> frame;
            if (frame.empty()) {
                break;
            }
            else src_ims.push_back(frame.clone());
        }
    }

    void videoshow(string path)
    {
        VideoCapture capture(path);
        while (1)
        {
            //frame存储每一帧图像
            Mat frame;
            //读取当前帧
            capture >> frame;
            //播放完退出
            if (frame.empty()) {
                printf("播放完成\n");
                break;
            }
            //显示当前视频
            imshow("读取视频", frame);
            //延时300ms
            waitKey(300);
        }


    }



}

namespace enAndde_code {

    //像素点大小
    const int PIX_SIZE = 8;

    //获取每一个信息块内黑白比例
    double whiteGrayscale_percent(Mat img, int x, int y) {
        int white = 0;
        for (int i = x; i < x + PIX_SIZE; i++) {
            for (int j = y; j < y + PIX_SIZE; j++) {
                if (img.at<uchar>(i, j) > 128) white++;
            }
        }
        return white / ((double)PIX_SIZE * PIX_SIZE);
    }

    //对图片内像素块进行遍历，若黑/白数量超过一定阈值，便判定为黑/白，否则为误码
    string print_01(Mat img, int x, int y) {
        string s = "";
        for (int i = x; i < x + 800; i += PIX_SIZE) {
            for (int j = y; j < y + 800; j += PIX_SIZE) {
                double rate = whiteGrayscale_percent(img, i, j);
                if (rate > 0.6) s += '1';
                else if (rate < 0.4)s += '0';
                else s += '-';
            }
        }
        return s;
    }

    //接口
    string decode(const Mat& img) {
        string rt = print_01(img, 16, 16);
        return rt;
    }
}


void test1() {

    //获取0-1串信息
    string s = encoding::getFile();
    s = encoding::file2Binary(s);

    //编码
    vector<string> str = encoding::split(s);

    vector<Mat> img_Set = encoding::string2Mat_vector(str);

    //输出视频
    picvideo::pic2video(img_Set);

}


void test2() {

    //手机拍摄视频的地址
    const string path = "E:\\test5.mp4";

    vector<Mat> img_set;

    picvideo::video2pic(path, img_set);

    vector<Mat> rec = module3::get_QRcode(img_set);


    fstream file("5.bin", ios::binary | ios::out);
    fstream vfile("v5.bin", ios::binary | ios::out);

    //记录每个标号的图片是否被读取过
    int used[5000] = { 0 };

    //从第一张图片开始读取
    int pic_tag = 1;

    for (int i = 0; i < rec.size(); i++) {
        
        //读取图片中的0-1串
        string s = enAndde_code::decode(rec[i]);
        int num = 0;

        //获得图片标号信息
        for (int j = 0; j < s.size() && j < 8; j++) {
            num *= 2;
            num += (s[j] == '1' ? 1 : 0);
            if (s[j] == '-') { num = 10000; break; }
        }

        //如果读取图片标号不正确
        if (num!= pic_tag)continue;

        //选取每个标号的第二张图片
        if (!used[num]) { used[num]++; continue; }

        //读取下一张图片
        pic_tag++;

        //解读信息，如果标记为'-'就当作误码处理
        for (int j = 8; j < s.size(); j+=8) {
            unsigned char ch = 0, lg = 0;
            bool flag = 1;
            for (int k = j + 7; k >= j; k--) {
                ch *= 2;
                ch += s[k] == '1' ? 1 : 0;
                lg *= 2;
                lg += s[k] == '-' ? 0 : 1;
            }
            file.write((char*)&ch, sizeof(ch));
            vfile.write((char*)&lg, sizeof(ch));
        }
    }
}


int main(){
    /******test1从二进制文件生成视频******/
    test1();

    /******test2从手机录屏解读信息******/
    //test2();

    return 0;
}
