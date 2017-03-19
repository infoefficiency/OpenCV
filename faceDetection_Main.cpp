#include "faceDetection.hpp"

int main() { 
	map<int, int> m;
	//Access Camera
	VideoCapture camera;
	camera.open(0);

	if (!camera.isOpened()) {
		cerr << "ERROR : Could not access the camera or video" << endl;
		exit(1);
	}

	while (true) {
		Mat img;
		camera >> img;
		if (img.empty()) {
			cerr << "ERROR : Could not grab a camera frame" << endl;
			exit(1);
		}
		
		string path = "C:/opencv/opencv_3.2/sources/data/haarcascades_cuda/";
		
		string filename = "haarcascade_frontalface_alt.xml";
		vector<Rect> frontFace = faceDetection(img, path, filename);
		drawRectangle(img, frontFace);
		imshow("image", img);
		waitKey(1);

	}
}