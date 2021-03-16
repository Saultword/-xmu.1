#include<iostream>
#include<opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<math.h>
#include<vector>
#include<strstream>
#include<fstream>
#include <windows.h>
#include <cstdlib>
#include <algorithm>
using namespace std;
using namespace cv;

int encode[8] = { 0 };
vector<Mat>Imagesbuffer;

char Cxor(char a, char b) {
	if (a == b) {
		return '0';

	}
	else if (a != b) {
		return '1';
	}
}

string CalculateCRC(string data, string& CRC) {
	data.append(string(CRC.size(), '0'));
	for (int i = 0; i < data.size() - CRC.size(); i++) {
		if (data[i] == '1') {
			for (int j = 0; j < CRC.size(); j++) {
				data[i + j] = Cxor(data[i + j], CRC[j]);
			}
		}
	}
	return data.substr(data.size() - CRC.size(), CRC.size());//功能不明，尚需研发
}
//画四个识别框
void draw4blocks(Mat Imagesbuffer, Point tr) {//用来画角落的矩形，画法是黑白黑3个矩形框叠加--现成的焦点dd
	rectangle(Imagesbuffer, Point(0, 0) + tr, Point(105, 105) + tr, Scalar(0, 0, 0), -1);
	rectangle(Imagesbuffer, Point(15, 15) + tr, Point(90, 90) + tr, Scalar(255, 255, 255), -1);
	rectangle(Imagesbuffer, Point(30, 30) + tr, Point(75, 75) + tr, Scalar(0, 0, 0), -1);
}

void Encode(unsigned char a) {//编码函数，将字符的ASCII码转2进制
	for (int j = 7; j >= 0; j--) {
		encode[j] = a % 2;
		a = a / 2;
	}
}

void Draw(string code, int lastimg_flag) {
	int key = 0;
	int one = 255, two = 255, three = 255;//三通道值
	int a, b;
	Mat src(780, 780, CV_8UC3, Scalar(255, 255, 255));//矩阵参数，长和高都是780，格式包括但不限于：_type is CV_8UC1, CV_64FC3, CV_32SC(12) ，scalar将所有矩形灰度值全设定为白即255
	draw4blocks(src, Point(0 + 30, 0 + 30));
	draw4blocks(src, Point(615 + 30, 0 + 30));
	draw4blocks(src, Point(0 + 30, 615 + 30));
	draw4blocks(src, Point(615 + 30, 615 + 30));//画4个识别框


	for (int col = 0; col < 36 * 2; col++) {//画最上面那块
		for (int row = 0; row < 24 * 2; row++) {
			Point pos(row * 10 + 120 + 30, col * 10 + 30);
			Point recSize(10, 10);
			a = code[key] - 48;
			b = code[key + 1] - 48;
			if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
			if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
			if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
			if (a == 1 && b == 1) { one = 255; two = 0; three = 0; }
			rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
			key = key + 2;
			if (lastimg_flag == 1 && key >= code.size())
				goto enddraw;
		}
	}

	for (int col = 0; col < 24 * 2; col++) {
		for (int row = 24 * 2; row < 30 * 2; row++) {
			Point pos(row * 10 + 120 + 30, col * 10 + 120 + 30);//因为只能从120开始画，前面是定位的方格
			Point recSize(10, 10);//pos和recSize都是point类
			a = code[key] - 48;
			b = code[key + 1] - 48;
			if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
			if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
			if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
			if (a == 1 && b == 1) { one = 255; two = 0; three = 0; }
			rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
			key = key + 2;
			if (lastimg_flag == 1 && key >= code.size())
				goto enddraw;
		}
	}

	for (int col = 0; col < 24 * 2; col++) {
		for (int row = 0; row < 6 * 2; row++) {
			Point pos(row * 10 + 30, col * 10 + 120 + 30);
			Point recSize(10, 10);
			a = code[key] - 48;
			b = code[key + 1] - 48;
			if (a == 0 && b == 0) { one = 0; two = 0; three = 0; }
			if (a == 0 && b == 1) { one = 0; two = 0; three = 255; }
			if (a == 1 && b == 0) { one = 0; two = 255; three = 0; }
			if (a == 1 && b == 1) { one = 255; two = 0; three = 0; }
			rectangle(src, pos, pos + recSize, Scalar(one, two, three), -1);//颜色1或者0，全填充
			key = key + 2;
			if (lastimg_flag == 1 && key >= code.size())
				goto enddraw;
		}
	}
enddraw:
	Imagesbuffer.push_back(src);
}

void Img2Video() {
	char videoname[20];
	cout << "请输入编码后的视频文件名+.avi（方便播放）:" << endl;
	cin >> videoname;
	int frame_rate;
	cout << "输入视频帧率"<<endl;
	cin >> frame_rate;
	
	VideoWriter Img2Video(videoname, CAP_ANY, frame_rate, Size(780, 780));//文件名，编码格式，帧率，大小
	for (size_t i = 0; i < Imagesbuffer.size(); i++)
	{
		Mat image = Imagesbuffer[i].clone();
		// 流操作符，把图片传入视频
		Img2Video << image;
	}
	cout << "转换成功" << endl;
}





void Encode_crc(string code, int lastimg_flag) {
	string CRC = "10000010011000001000111011011011";
	code.append(CalculateCRC(code, CRC));
	Draw(code, lastimg_flag);
}

int main()
{
	
	
	 {
		char name1[20];
		cout << "请输入需要转换的文件名（建议bin或者txt）:" << endl;
		cin >> name1;
		//信息读入
		vector<int>out;
		string put_in_string;//输入文件流
		ifstream fs(name1, ios::binary);//当前目录下读入
		stringstream ss;
		ss << fs.rdbuf();//文件流转字符流
		put_in_string = ss.str();//临时将ss流中字符串转换string类型变量
		int length_char = put_in_string.size();//求字符串长度
		int length_bit = length_char * 8;

		//信息到二进制-编码部分
		int img_count;
		int LEN = 8;
		img_count = length_bit / 9184;//判断要做几张图
		if (length_bit % 9184 != 0) img_count++;
		//cout << img_count << endl;
		//cout << length_char << endl;
		string data;
		int temporary;
		put_in_string += "\0";
		if (length_char > 92000)length_char = 92000;
		if (img_count > 80)img_count = 81;
		for (int i = 0; i < length_char; i++) {
			Encode((unsigned char)put_in_string[i]);//编码函数，设置全局变量 encode[LEN]来传递
			for (int j = 0; j < LEN; j++)
			{
				temporary = encode[j];
				if (temporary == 1) data.append(1, '1');
				if (temporary == 0) data.append(1, '0');
			}
		}
		data += "\0";
		
		for (int i = 0; i < img_count - 1; i++) {
			Encode_crc(data.substr(i * 1148 * 4 * 2, 1148 * 4 * 2), 0);//第二个参数为0表示不是最后一张图
		}
		Encode_crc(data.substr((img_count - 1) * 1148 * 4 * 2), 1);
		
		Img2Video();
	}
	
}