#include"picvideo.h"

namespace picvideo {

    //将元组内各类图片转化为视频
    void pic2video(vector<Mat>& src_ims)
    {
        string name2 = "test.avi";
        cout << "请输入编码后的视频文件名，例如“test.avi”:" << endl;
        cin >> name2;

        int frame_rate = 15;
        //cout << "请输入视频帧率:" << endl;
        //cin >> frame_rate;

        VideoWriter video(name2, VideoWriter::fourcc('M', 'P', '4', 'V'), frame_rate, Size(1000, 1000), false);
        for (size_t i = 0; i < src_ims.size(); i++)
        {
            Mat image = src_ims[i].clone();
            // 流操作符，把图片传入视频
            video << image;
        }

    }

    //将视频逐帧提取进入元组成为Mat类对象
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

    //用于播放视频内容，参数为当目录下文件名字
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
}
