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
    string getFile();

    //文件转化成二进制
    string file2Binary(string s);

    //待写入文件分块
    vector<string> split(string s);

    //将string内存的二进制信息存入Mat
    Mat string2Mat(string s, int num);

    //批量处理
    vector<Mat> string2Mat_vector(vector<string> s);
}