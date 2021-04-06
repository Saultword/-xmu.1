#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<math.h>
#include<vector>
#include<strstream>
#include<fstream>
using namespace std;
using namespace cv;

namespace module3{

    //增强图像 //进行二值化操作
    Mat enhancement(Mat src);

    //Canny算法，Canny阈值输入比例1:3
    Mat CannyThreshold(Mat& src);

    //对图片进行膨化和腐蚀处理，去除不需要的边缘和闭合间隙
    void dilate_erode(Mat& src, Mat& dst);


    //确定目标图片中的目标二维码所在区域
    vector<Point> getRect(Mat& src, Mat& dst);

    //透视映射，返回映射后的图片
    Mat get_Mappedim(Mat src, vector<Point> point);

    //module3的接口
    vector<Mat> get_QRcode(vector<Mat>& src_ims);
}