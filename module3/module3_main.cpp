#include "module3.h"

using namespace module3;

int main()
{
	char window_name[15] = "Module3_2Dcode";

	//创建显示窗口
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	//创建一个图片流输入、输出接口
	vector<Mat> src_images;

	//测试接口
	Mat src_image = imread("./test.jpg");

	src_images.push_back(src_image);

	src_images = get_QRcode(src_images);
	

	//显示结果
	imshow(window_name, src_images[0]);
	imwrite("C:/Users/df/Desktop/duibi.jpg", src_images[0]);
	// 等待用户反应
	waitKey(0);

	return 0;
}