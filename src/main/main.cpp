/*******include path ignored**************/
void test1() {
    string s = encoding::getFile();
    s = encoding::file2Binary(s);
    
    freopen("in.txt", "w", stdout);

    cout << s;

    vector<string> str = encoding::split(s);

    vector<Mat> img_Set = encoding::string2Mat_vector(str);

    picvideo::pic2video(img_Set);

}


void test2() {
    const string path = "E:\\test2.mp4";          //测试视频位置

    vector<Mat> img_set;

    picvideo::video2pic(path, img_set);

    vector<Mat> rec = module3::get_QRcode(img_set);

    freopen("out.txt", "w", stdout);

    int used[5000] = { 0 };                       //防止某帧重复读取

    int now = 1;

    for (int i = 0; i < rec.size(); i++) {
        //if (i % 2)continue;
        string s = enAndde_code::decode(rec[i]);
        int num = 0;
        for (int j = 0; j < s.size() && j < 8; j++) {         //获取编号
            num *= 2;
            num += (s[j] == '1' ? 1 : 0);
            if (s[j] == '-') { num = 10000; break; }
        }
        if (s.size() < 10000)continue;
        if (num!=now||used[num])continue;                     //顺序读取
        used[num] = 1; now++;
        string ot;
        for (int j = 8; j < s.size(); j++)ot += s[j];       //除去开头编号
        cout << ot;
        /*stringstream number;
        number << num;
        string name;
        number >> name;
        name += ".jpg";
        imwrite(name, rec[i]);*/
    }
}

int main()
{

    //test1();
    test2();

    return 0;
}
