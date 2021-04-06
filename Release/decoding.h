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

namespace decoding {

    //像素点大小
    const int PIX_SIZE = 8;

    //获取每一个信息块内黑白比例
    double whiteGrayscale_percent(Mat img, int x, int y);

    //对图片内像素块进行遍历，若黑/白数量超过一定阈值，便判定为黑/白，否则为误码
    string print_01(Mat img, int x, int y);

    //单张图片解码
    string decode(const Mat& img);

    //接口
    void decoding(const vector<Mat>& rec);
}