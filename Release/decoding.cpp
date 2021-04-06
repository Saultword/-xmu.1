#include"decoding.h"


namespace decoding {

    //获取每一个信息块内黑白比例
    double whiteGrayscale_percent(Mat img, int x, int y) {
        int white = 0;
        for (int i = x; i < x + PIX_SIZE; i++) {
            for (int j = y; j < y + PIX_SIZE; j++) {
                if (img.at<uchar>(i, j) > 128) white++;
            }
        }
        return white / ((double)PIX_SIZE * PIX_SIZE);
    }

    //对图片内像素块进行遍历，若黑/白数量超过一定阈值，便判定为黑/白，否则为误码
    string print_01(Mat img, int x, int y) {
        string s = "";
        for (int i = x; i < x + 800; i += PIX_SIZE) {
            for (int j = y; j < y + 800; j += PIX_SIZE) {
                double rate = whiteGrayscale_percent(img, i, j);
                if (rate > 0.6) s += '1';
                else if (rate < 0.4)s += '0';
                else s += '-';
            }
        }
        return s;
    }

    //单张图片解码
    string decode(const Mat& img) {
        string rt = print_01(img, 16, 16);
        return rt;
    }

    //接口
    void decoding(const vector<Mat>& rec) {

        string path2 = "1.bin", path3 = "v1.bin";

        cout << "请输入信息文件名称" << endl;
        cin >> path2;
        cout << "请输入校验文件名称" << endl;
        cin >> path3;

        fstream file(path2, ios::binary | ios::out);
        fstream vfile(path3, ios::binary | ios::out);

        //记录每个标号的图片是否被读取过
        int used[5000] = { 0 };

        //从第一张图片开始读取
        int pic_tag = 1;

        for (int i = 0; i < rec.size(); i++) {

            //读取图片中的0-1串
            string s = decoding::decode(rec[i]);
            int num = 0;

            //获得图片标号信息
            for (int j = 0; j < s.size() && j < 8; j++) {
                num *= 2;
                num += (s[j] == '1' ? 1 : 0);
                if (s[j] == '-') { num = 10000; break; }
            }

            //如果读取图片标号不正确
            if (num != pic_tag)continue;

            //选取每个标号的第二张图片
            if (!used[num]) { used[num]++; continue; }

            //读取下一张图片
            pic_tag++;

            //解读信息，如果标记为'-'就当作误码处理
            for (int j = 8; j < s.size(); j += 8) {
                unsigned char ch = 0, lg = 0;
                bool flag = 1;
                for (int k = j + 7; k >= j; k--) {
                    ch *= 2;
                    ch += s[k] == '1' ? 1 : 0;
                    lg *= 2;
                    lg += s[k] == '-' ? 0 : 1;
                }
                file.write((char*)&ch, sizeof(ch));
                vfile.write((char*)&lg, sizeof(ch));
            }
        }
    }

}