#include"main.h"

void test1() {

    //获取0-1串信息
    string s = encoding::getFile();
    s = encoding::file2Binary(s);

    //编码
    vector<string> str = encoding::split(s);

    vector<Mat> img_Set = encoding::string2Mat_vector(str);

    //输出视频
    picvideo::pic2video(img_Set);

}


void test2() {

    //手机拍摄视频的地址
    cout << "请输入手机拍摄的视频名称" << endl;
    string path1 = "E:\\test5.mp4";
    cin >> path1;

    vector<Mat> img_set;

    picvideo::video2pic(path1, img_set);

    vector<Mat> rec = module3::get_QRcode(img_set);
    
    decoding::decoding(rec);
}


int main()
{
    /******test1从二进制文件生成视频******/
    //test1();

    /******test2从手机录屏解读信息******/
    test2();

    return 0;
}
