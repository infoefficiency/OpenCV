#include "use_opencv.h"
#include <iostream>

using namespace cv;
using namespace std;

void cartoonifyImage(Mat& srcColor, Mat& dst) {
	
	Mat gray;	
	cvtColor(srcColor, gray, CV_BGR2GRAY);
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
	Mat edges;
	const int LAPLACIAN_FILTER_SIZE = 5;
	Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
	
	Mat mask;
	const int EDGES_THRESHOLD = 80;
	threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
	
	Size size = srcColor.size();
	Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;
	Mat smallImg = Mat(smallSize, CV_8UC3);
	resize(srcColor, smallImg, smallSize, 0, 0, INTER_LINEAR);

	Mat tmp = Mat(smallSize, CV_8UC3);
	int repetitious = 10;
	for (int i = 0; i < repetitious; ++i) {
		int ksize = 9;
		double sigmaColor = 9;
		double sigmaSpace = 7;
		bilateralFilter(smallImg, tmp, ksize, sigmaColor, sigmaSpace);
		bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);
		
		Mat bigImg;
		resize(smallImg, bigImg, size, 0, 0, INTER_LINEAR);
		dst.setTo(0);
		bigImg.copyTo(dst, mask);
	}
}

void imageProcess(const string& address) {
	Mat img = imread(address, IMREAD_COLOR);
	Mat rst = Mat(img.size(), CV_8UC3);
	cartoonifyImage(img, rst);

	imshow("img", rst);
	waitKey(0);
}

void videoProcess() {
	int cameraNumber = 0;
	VideoCapture camera;
	camera.open(cameraNumber);
	if (!camera.isOpened()) {
		cerr << "ERROR : Could not access the camera or video!" << endl;
		exit(1);
	}

	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);

	while (true) {
		Mat cameraFrame;
		camera >> cameraFrame;
		if (cameraFrame.empty()) {
			cerr << "ERROR : Could not grab a camera frame." << endl;
			exit(1);
		}

		//빈 결과 영상을 생성한 후 그 영상에 그립니다.
		Mat displayedFrame(cameraFrame.size(), CV_8UC3);

		//카메라 프레임에 만화 생성기 필터를 실행합니다.
		cartoonifyImage(cameraFrame, displayedFrame);

		imshow("Cartoonifier", displayedFrame);
		char keypress = waitKey(1);
		if (keypress == 27) {
			break;
		}
	}
}
int main() {
	imageProcess("d:/image/ioniq.jpg");
	videoProcess();
}