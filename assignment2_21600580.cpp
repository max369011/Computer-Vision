#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat img, negative_img, log_img, gamma_img;

	img = imread("lena.png", 0);
	imshow("original_img", img);

	if (img.empty()) {
		cout << "No such image file" << endl;
		return -1;
	}

	//negative image
	negative_img = 255 - img;
	imshow("negative_img", negative_img);

	//log image
	double c = 1.5;
	img.convertTo(log_img, CV_32F);
	log_img += 1;
	log(log_img, log_img);
	normalize(log_img, log_img, 0, 255, NORM_MINMAX);
	convertScaleAbs(log_img, log_img, c, 0);
	imshow("log_img", log_img);

	//gamma image
	double gamma = 0.5;
	double consnt = 0.75;

	img.convertTo(gamma_img, CV_32F);
	normalize(gamma_img, gamma_img, 0, 1.0, NORM_MINMAX);
	pow(gamma_img, gamma, gamma_img);
	normalize(gamma_img, gamma_img, 0, 255, NORM_MINMAX);
	convertScaleAbs(gamma_img, gamma_img, consnt, 0);

	imshow("gamma_img", gamma_img);
	
	waitKey(0);
	return 0;
}