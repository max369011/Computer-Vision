#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat lena_img, avg_img, lena_roi, moon_img, moon_laplacian, moon_roi, moon_sharpening, saltnpepper_img, saltnpepper_filtered, empty, mask;

	
	//Averaging filtering left half
	lena_img = imread("lena.png");

	if (lena_img.empty()) {
		cout << "Error reading image file" << endl;
		return -1;
	}
	imshow("lena", lena_img);

	Rect rect(0, 0, (lena_img.size().width / 2), lena_img.size().height);
	lena_roi = lena_img(rect);
	blur(lena_roi, lena_roi, Size(7, 7));
	lena_img.copyTo(lena_roi);

	imshow("lena_filtered", lena_img);

	//Sharpening right half
	moon_img = imread("moon.jpg");
	if (moon_img.empty()) {
		cout << "Error reading image file" << endl;
		return -1;
	}
	imshow("moon", moon_img);

	moon_roi = moon_img.clone();
	Laplacian(moon_roi, moon_laplacian, CV_16S);
	convertScaleAbs(moon_laplacian, moon_laplacian);

	mask = Mat::zeros(moon_img.size(), moon_img.type());
	Rect rect3 = Rect((moon_img.size().width/2) ,0, (moon_img.size().width / 2), moon_img.size().height);
	rectangle(mask, rect3, Scalar(255,255,255), -1);
	moon_laplacian.copyTo(empty, mask);

	moon_sharpening = empty + moon_img;
	imshow("moon_filtered", moon_sharpening);
	

	//Median filtering
	saltnpepper_img = imread("saltnpepper.png");
	if (saltnpepper_img.empty()) {
		cout << "Error reading image file" << endl;
		return -1;
	}
	imshow("saltnpepper", saltnpepper_img);

	medianBlur(saltnpepper_img, saltnpepper_filtered, 9);
	imshow("saltnpepper_filtered", saltnpepper_filtered);
	


	waitKey(0);
	return 0;
}