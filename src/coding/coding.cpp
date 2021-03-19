#include "coding.h"
using namespace std;
using namespace cv;

namespace encoding {

    //单张图片存储二进制位最大值
    const int MAX_SIZE = 100 * 100 - 8;

    //读文件+返回string
    string getFile() {

        //打开文件
        string rt;
        FILE* fp = fopen("data.in","r");
        if (fp == nullptr) {
            cout << "Open file failed! " << endl;
            return rt;
        }

        //读取文件
        char ch;
        while ((ch = fgetc(fp)) != EOF)rt += ch;

        return rt;
    }

    //文件转化成二进制
    string file2Binary(string s) {
        string rt;
        int len = s.length();
        for (int i = 0; i < len; i++) {
            char ch = s[i];
            for (int j = 0; j < 8; j++) {
                if (ch % 2)rt += '1';
                else rt += '0';
                ch /= 2;
            }
        }
        return rt;
    }

    //待写入文件分块
    vector<string> split(string s) {
        vector<string> rt;
        int len = s.size();
        int head = 0, end = min(head + MAX_SIZE, len);
        while (head != len) {
            string tmp;
            for (int i = head; i < end; i++) {
                tmp.push_back(s[i]);
            }
            rt.push_back(tmp);
            head = end;
            end = min(head + MAX_SIZE, len);
        }
        return rt;
    }

    //将string内存的二进制信息存入Mat
    Mat string2Mat(string s, int num) {
        Mat img(100, 100, CV_8UC1);

        //存入张数信息
        for (int i = 0; i <= 7; i++) {
            img.ptr<uchar>(0)[i] = (num % 2 == 1 ? 255 : 0);
        }

        //存入数据
        int cnt = 0, len = s.size();
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                if (i == 0 && j < 8)continue;
                if (cnt >= len)img.ptr<uchar>(i)[j] = 0;
                img.ptr<uchar>(i)[j] = (s[cnt++] == '0' ? 255 : 0);
            }
        }

        //放大图片
        Mat bigger_img(800, 800, CV_8UC1);

        for (int i = 0; i < 800; i++) {
            for (int j = 0; j < 800; j++) {
                int x = (i % 400) / 4;
                int y = (j % 400) / 4;
                bigger_img.ptr<uchar>(i)[j] = img.ptr<uchar>(x)[y];
            }
        }


        //添加轮廓
        Mat rt(1000, 1000, CV_8UC1);

        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++) {
                rt.ptr<uchar>(i)[j] = 255;
                if (i <= 905 && i >= 95 && j <= 905 && j >= 95) {
                    rt.ptr<uchar>(i)[j] = 0;
                }
            }
        }

        //往轮廓内添加信息
        for (int i = 100; i < 900; i++) {
            for (int j = 100; j < 900; j++) {
                rt.ptr<uchar>(i)[j] = bigger_img.ptr<uchar>(i - 100)[j - 100];
            }
        }

        return rt;
    }

    //批量处理
    vector<Mat> string2Mat_vector(vector<string> s) {
        int len = s.size();
        vector<Mat> rt;
        for (int i = 0; i < len; i++) {
            Mat tmp = string2Mat(s[i], i + 1);
            rt.push_back(tmp);
        }
        return rt;
    }
}
