#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Mat finger_print, adaptive_1, adaptive;

	finger_print = imread("finger_print.png", 0);
	adaptive_1 = imread("adaptive_1.jpg", 0);
	adaptive = imread("adaptive.png", 0);

	if (finger_print.empty() || adaptive_1.empty() || adaptive.empty()) {
		cout << "Error reading image file" << endl;
		return -1;
	}

	//threshold for finger_print
	threshold(finger_print, finger_print, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//inRange(finger_print, 128, 255, finger_print);

	//adaptive threshold for adaptive_1
	adaptiveThreshold(adaptive_1, adaptive_1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15);
	
	//adaptive threshold for adaptive	
	adaptiveThreshold(adaptive, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 15);

	imshow("finger_print", finger_print);
	imshow("adaptive_1", adaptive_1);
	imshow("adaptive", adaptive);

	waitKey(0);
	return 0;
}