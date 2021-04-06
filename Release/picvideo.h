#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<vector>
#include<strstream>
#include<fstream>
using namespace std;
using namespace cv;

namespace picvideo {

    //将元组内各类图片转化为视频
    void pic2video(vector<Mat>& src_ims);

    //将视频逐帧提取进入元组成为Mat类对象
    void video2pic(string path, vector<Mat>& src_ims);

    //用于播放视频内容，参数为当目录下文件名字
    void videoshow(string path);
}
