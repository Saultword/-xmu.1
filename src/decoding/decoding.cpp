#include "decoding.h"

using namespace enAndde_code;

int main() {
//	ofstream infile("..\\in.txt", ios::out);								//这两行用于随机生成txt文件
//	form(infile);															//用的时候将其他行注释掉（下四行）
	ifstream infile("..\\in.txt", ios::in);
	ofstream outfile("..\\out.txt", ios::out && ios::binary);
	encode(infile);															//将infile中的内容编码，生成二维码
	decode(outfile);														//将二维码解码，并写入到outfile中
	outfile.close();		
	infile.close();
	return 0;
}

