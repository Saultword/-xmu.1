#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <fstream>
#include <iostream>
#include <bitset>
#include <stdio.h>
#include <vector>
#define charNumber 5000												//字符个数，最多5000个
using namespace std;
using namespace cv;

namespace enAndde_code {
    vector<string> pool;
    double whiteGrayscale_percent(Mat img, int x, int y) {
        int white = 0;
        for (int i = x; i < x + 8; i++) {
            for (int j = y; j < y + 8; j++) {
                if (img.at<uchar>(i, j) > 128) white++;
            }
        }
        return white / 64.0;
    }
    string print_01(Mat img, int x, int y) {
        string s = "";
        for (int i = x; i < x + 800; i += 8) {
            for (int j = y; j < y + 800; j += 8) {
                double rate = whiteGrayscale_percent(img, i, j);
                if (rate > 0.6) s += '1';
                else if (rate < 0.4)s += '0';
                else s += '-';
            }
        }
        return s;
    }

    string decode(const Mat& img) {									//解码部分,将解码后的信息存放在ofile中  decode(ofstream& ofile_01)
        string rt = print_01(img, 16, 16);
        return rt;
    }
}
