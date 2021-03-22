#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui_c.h>
#include "opencv2/highgui/highgui.hpp"
#include <vector>

using namespace std;
using namespace cv;

namespace module3
{
    // 命名空间成员函数 -- 声明


    //Canny算法，Canny阈值输入比例1:3
    Mat CannyThreshold(Mat& src);

    //对图像进行二值化处理
    Mat enhancement(Mat src);

    //对图像进行修复
    Mat repair(Mat src);
    //对图片进行膨化和腐蚀处理，去除不需要的边缘和闭合间隙
    void dilate_erode(Mat& src, Mat& dst);

    //修改矩形的坐标，
    //void reSizeRect(Rect& rect);


    //确定目标图片中的目标二维码所在区域
    vector<Point> getRect(Mat& src, Mat& dst);

    Mat get_Mappedim(Mat src, vector<Point> point);

    vector<Mat> get_QRcode(vector<Mat>& src_ims);

}