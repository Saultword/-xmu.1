#include "module3.h"
#include<iostream>

using namespace module3;
using namespace std;

int main()
{
	char window_name[15] = "Module3_2Dcode";

	//创建一个图片流输入、输出接口
	vector<Mat> src_images;
	vector<Mat> dst_images;

	//单张图片测试接口
	/*
	//创建显示窗口
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);



	Mat src_image = imread("./2.jpg");
	src_images.push_back(src_image);
	src_images[0] = enhancement(src_images[0]);

	//显示结果
	imshow(window_name, src_images[0]);
	imwrite("./duibi2.jpg", src_images[0]);
	*/

	//图片流测试接口
	String pattern_jpg = "./src_image/*.jpg";
	vector<String>image_files;
	glob(pattern_jpg, image_files);
	for (int i = 0; i < image_files.size(); i++)
	{
		Mat tmp = imread(image_files[i]);
		if (tmp.data)
			src_images.push_back(tmp);
	}
	
	dst_images = get_QRcode(src_images);

	for (int i = 0; i < dst_images.size(); i++)
	{
		dst_images[i] = enhancement(dst_images[i]);
		char str[10];
		sprintf_s(str, "%u.jpg", i + 1);
		string tmp = "./dst_image/";
		string strSaveName = tmp + str;
		cout << strSaveName << endl;
		imwrite(strSaveName , dst_images[i]);
	}
	

	cout << "程序结束！" << endl;
	// 等待用户反应
	waitKey(0);

	return 0;
}