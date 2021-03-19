#include "coding.h"
using namespace std;

namespace coding {

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

    }

}
