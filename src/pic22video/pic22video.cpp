#include<iostream>
#include "pic22video.h"
using namespace std;
using namespace cv;


using namespace picvideo;

	void pic2video(vector<Mat>& src_ims)
	{
		char name2[20];
		cout << "请输入编码后的视频文件名，例如“test.mp4”:" << endl;
		cin >> name2;
		
		cout << "请输入视频帧率:" << endl;
		int frame_rate;
		cin >> frame_rate;
		VideoWriter video(name2, CAP_ANY, frame_rate, Size(1000, 1000));
		for (size_t i = 0; i < src_ims.size(); i++)
		{
			if(i==src_ims.size())
			{
				for(int j=0;j<frame_rate;j++)
				{
					Mat image = src_ims[i].clone();
			                video << image;
				}
			}
			else
			{Mat image = src_ims[i].clone();
			// 流操作符，把图片传入视频
			video << image;}
		}
	}
	void video2pic(string path, vector<Mat>& src_ims)
	{
		VideoCapture capture(path);
		Mat frame;
		while (true) {
			//一帧一帧读
			capture >> frame;
			if (frame.empty()) {
				break;
			}
			else src_ims.push_back(frame.clone());
		}
	}

	void videoshow(string path)
	{
		VideoCapture capture(path);
		while (1)
		{
			//frame存储每一帧图像
			Mat frame;
			//读取当前帧
			capture >> frame;
			//播放完退出
			if (frame.empty()) {
				printf("播放完成\n");
				break;
			}
			//显示当前视频
			imshow("读取视频", frame);
			//延时300ms
			waitKey(300);
		}

		
	}





