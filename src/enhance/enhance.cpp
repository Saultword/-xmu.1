#include"enhance.h"
using namespace std;
using namespace cv;

namespace enhance {

	//增强图像
	Mat enhancement(Mat src) {
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; i < src.cols; j++) {
				src.ptr<uchar>(i)[j] = (src.ptr<uchar>(i)[j] > 128 ? 255 : 0);
			}
		}
		return src;
	}
}
