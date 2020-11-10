#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat CannyEdge(Mat img) {

	Mat grad_x, grad_y, result;

	cvtColor(img, img, CV_BGR2GRAY);
	
	blur(img, img, Size(5, 5));
	//GaussianBlur(img, img, Size(5, 5), 5, 5, BORDER_DEFAULT);

	Canny(img, img, 10, 60, 3);

	return img;
}


int main() {
	Mat frame, left_roi, right_roi;
	VideoCapture cap;

	if (cap.open("Road.mp4") == 0) {
		cout << "Cannot open video file" << endl;
		waitKey(0);
	}

	double fps = cap.get(CAP_PROP_FPS);
	Rect left_rect(200, 400, 400, 200);
	Rect right_rect(600, 400, 400, 200);

	while (1) {
		cap >> frame;

		if (frame.empty()) {
			cout << "End of Video" << endl;
			break;
		}

		left_roi = frame(left_rect);
		right_roi = frame(right_rect);

		left_roi = CannyEdge(left_roi);
		right_roi = CannyEdge(right_roi);

		namedWindow("Left canny");
		moveWindow("Left canny", 200, 0);
		imshow("Left canny", left_roi);
		
		namedWindow("Right canny");
		moveWindow("Right canny", 600, 0);
		imshow("Right canny", right_roi);
	
		waitKey(1000 / fps);
	}

	waitKey(0);
	return 0;
}