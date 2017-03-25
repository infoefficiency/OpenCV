#pragma once

#include "opencv.hpp"
using namespace cv;
using namespace std;

/*
--- faceDetection ---
This function detects faces and return the position with vector<Rect> type.
It includes position of face as Rect type.
--- Input ---
1) Mat &img : image
2) string &filePath : classifier directory path
3) string &filenane : classifier name(.xml)
4) bool singular : detect one face(1) / all face (0)
*/
vector<Rect> faceDetection(Mat& img, string& filePath, string& filename, bool singular = true) {

	string faceCascadeFilename = filePath + filename;
	//Include Haar/LBP classifier for detecting face
	CascadeClassifier faceDetector;
	try {
		faceDetector.load(faceCascadeFilename);
	}
	catch (Exception e) {}

	if (faceDetector.empty()) {
		cerr << "ERROR : Couldn't load Face Detector (";
		cerr << faceCascadeFilename << ")" << endl;
		exit(1);
	}

	// convert color to grayscale
	Mat gray;
	if (img.channels() == 3) {
		cvtColor(img, gray, CV_BGR2GRAY);
	}
	else if (img.channels() == 4) {
		cvtColor(img, gray, CV_BGRA2GRAY);
	}
	else {
		// direct access to grayscale
		gray = img;
	}

	//reduce camera image
	const int DETECTION_WIDTH = 320;
	Mat smallImg;
	float scale = gray.cols / (float)DETECTION_WIDTH;
	if (gray.cols > DETECTION_WIDTH) {
		// reduce the image for maintaining the identical aspect ratio
		int scaledHeight = cvRound(gray.rows / scale);
		resize(gray, smallImg, Size(DETECTION_WIDTH, scaledHeight));
	}
	else {
		smallImg = gray;
	}

	//Histrogram equalization
	Mat equalizedImg;
	equalizeHist(smallImg, equalizedImg);

	// fece detection
	int flags;
	if (singular) {
		//search one face
		flags = CASCADE_FIND_BIGGEST_OBJECT | CASCADE_DO_ROUGH_SEARCH;
	}
	else {
		// search many faces
		flags = CASCADE_SCALE_IMAGE;
	}

	Size minFeatureSize(20, 20); // small size face
	float searchScaleFactor = 1.1f;	// search size
	int minNeighbors = 4;	// reliability (normal : 3, > 3 : reliability goes up, small number of faces)

							// detect object in small grayscale
	vector<Rect> faces;
	faceDetector.detectMultiScale(equalizedImg, faces, searchScaleFactor, minNeighbors, flags, minFeatureSize);

	Mat restoreImg;
	if (gray.cols > DETECTION_WIDTH) {
		// reduce the image for maintaining the identical aspect ratio
		int scaledHeight = cvRound(gray.rows / scale);

		resize(equalizedImg, restoreImg, Size(img.cols, img.rows));

		int scaledWidth = 320;
		if (img.cols > scaledWidth) {
			for (int i = 0; i < (int)faces.size(); ++i) {
				faces[i].x = cvRound(faces[i].x * scale);
				faces[i].y = cvRound(faces[i].y * scale);
				faces[i].width = cvRound(faces[i].width * scale);
				faces[i].height = cvRound(faces[i].height * scale);
			}
		}

		for (int i = 0; i < (int)faces.size(); ++i) {
			if (faces[i].x < 0) {
				faces[i].x = 0;
			}
			if (faces[i].y < 0) {
				faces[i].y = 0;
			}
			if (faces[i].x + faces[i].width > img.cols) {
				faces[i].x = img.cols - faces[i].width;
			}
			if (faces[i].y + faces[i].height > img.rows) {
				faces[i].y = img.rows - faces[i].height;
			}
		}
	}

	return faces;

}

/*
--- drawRectangle ---
This function draw rectangle in image.
--- input ---
1) Mat& img : image matrix for drawing rectangle
2) vector<Rect> &object : It includes Rect information
3) Scalar color : It defines rectangle color (default : yellow)
4) int thickness : It defines thickness of rectangle (default : 2)
*/
void drawRectangle(Mat& img, vector<Rect> &object, Scalar color = Scalar(0, 255, 255), int thickness = 2) {
	for (int i = 0; i < (int)object.size(); ++i) {
		rectangle(img, object[i], color, thickness);
	}
}