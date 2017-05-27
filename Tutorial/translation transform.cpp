#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

	Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
	Mat dst(src.rows, src.cols, src.type(), Scalar(0));

	int dy = 30, dx = 40;
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			int y = i + dy;
			int x = j + dx;

			if (y < 0 || y >= src.rows) continue;
			if (x < 0 || x >= src.cols) continue;

			dst.at<uchar>(y, x) = src.at<uchar>(i, j);
		}
	}
	
	imshow("src", src);
	imshow("dst", dst);

	waitKey();

	return 0;
	
}
