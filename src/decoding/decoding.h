#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include <fstream>
#include <iostream>
#include <bitset>
#include <stdio.h>
#define charNumber 5000												//字符个数，最多5000个
using namespace std;
using namespace cv;

namespace enAndde_code {
	bitset<8> toBinary(char i) {									//将字符转化为二进制
		bitset<8> bit;
		bit = i;
		return bit;
	}

	char toDecimal(bitset<8> a) {									//将二进制转化为字符的AscII码
		char c = 0;
		for (int i = 0; i < 8; i++) {
			c += a[i] * pow(2, i);
		}
		return c;
	}

	void form(ofstream& ofile) {									//随机生成含有charNumber个AscII码为
		srand(time(NULL));											//48-122之间（键盘可输入）的字符的文件
		for (int i = 0; i < charNumber; i++) {
			char c = 48 + rand() % 74;
			ofile << c;
		}
	}

	void encode(ifstream& ifile) {									//编码部分，对ifile中的信息进行编码
		bitset<8> toBinary(char i);
		Mat	img = Mat::zeros(810, 810, CV_8UC1);
		char c;
		int colume = 0;
		int row = 0;
		while (ifile.peek() != EOF) {
			ifile >> c;
			bitset<8> ch = toBinary(c);
			for (int i = 0; i < 8; i++) {
				if (colume >= 200) {								//每一行存储200位信息，一行存完信息换下一行储存
					colume = 0;
					row++;
				}
				if (ch[i] == 0) {
					Rect rec1 = Rect(5 + 4 * colume, 5 + 4 * row, 4, 4);
					rectangle(img, rec1, Scalar(255), -1, 8, 0);
				}
				colume++;
			}
		}
		imshow("img", img);
		waitKey();
		imwrite("..\\二维码.jpg", img);
	}

	void decode(ofstream& ofile) {									//解码部分,将解码后的信息存放在ofile中
		Mat img = imread("..\\二维码.jpg", CV_8UC1);					//读取单通道图
		int Grayscale;												//Grayscale灰度。
		bitset<8> bitt;
		int count = 0;												//计数，每读取8位，输出一个字符
		for (int i = 6; i <= img.rows - 5; i += 4)					//因为每4×4个像素存一位信息，
			for (int j = 6; j <= img.cols - 5; j += 4)				//因此每4列，每4行读取一位。
			{
				Grayscale = img.at<uchar>(i, j);					//用at函数获取像素的灰度值
				if (count < 8) {
					if (Grayscale > 128) {
						bitt[count] = 0;
						count++;
					}
					else {
						bitt[count] = 1;
						count++;
					}
				}
				else {
					ofile << toDecimal(bitt);						//每读取8位，输出一个字符
					count = 0;
					if (Grayscale > 128) {
						bitt[count] = 0;
						count++;
					}
					else {
						bitt[count] = 1;
						count++;
					}

				}
			}
		ofile << toDecimal(bitt);
	}
}
