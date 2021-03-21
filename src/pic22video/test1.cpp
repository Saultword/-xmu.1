#include<iostream>
#include"pic22video.h"

using namespace std;
using namespace cv;
using namespace picvideo;

vector<Mat>Imagesbuffer;

int main()
{
	
	
	 
		string videoname;
		cin >> videoname;//输入要播放的视频名字；
		
		video2pic(videoname, Imagesbuffer);//将视频图片内容输入元组；
		cout << Imagesbuffer.size() << endl;//提示元组数量
		pic2video(Imagesbuffer);//元组内图片转视频重新编码
		
		
	
	
}