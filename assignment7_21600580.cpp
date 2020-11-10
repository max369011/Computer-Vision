#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat CannyEdge(Mat img) {

	Mat grad_x, grad_y, result;

	cvtColor(img, img, CV_BGR2GRAY);

	blur(img, img, Size(5, 5));
	//GaussianBlur(img, img, Size(5, 5), 5, 5, BORDER_DEFAULT);

	Canny(img, img, 50, 200, 3);

	return img;
}


int main() {
	Mat frame, left_roi, right_roi;
	VideoCapture cap;

	vector<Vec2f> lines;
	float rho, theta, a, b, x0, y0, rho_sum = 0, theta_sum = 0, rho_avg = 0, theta_avg = 0;
	int count = 0;
	Point p1, p2;
	Mat result;

	
	if (cap.open("Road.mp4") == 0) {
		cout << "Cannot open video file" << endl;
		waitKey(0);
	}

	double fps = cap.get(CAP_PROP_FPS);
	Rect left_rect(200, 400, 400, 200);
	Rect right_rect(600, 400, 400, 200);

	while (1) {
		cap >> frame;
		rho_sum = 0, theta_sum = 0, count = 0;
		
		//end condition
		if (cap.get(CAP_PROP_POS_MSEC) > 20000) {
			cout << "End of Video" << endl;
			break;
		}
		//roi of frame
		left_roi = frame(left_rect);
		right_roi = frame(right_rect);

		//edge detection for roi
		left_roi = CannyEdge(left_roi);
		right_roi = CannyEdge(right_roi);
		
		//line detection for left roi
		HoughLines(left_roi, lines, 1, CV_PI / 180, 100);

		for (int i = 0; i < lines.size(); i++) {
			rho = lines[i][0];
			theta = lines[i][1];

			//line filtering (angle 30~60 degrees)
			if (theta * (180 / CV_PI) >= 30 && theta * (180 / CV_PI) <= 60) {
				rho_sum += rho;
				theta_sum += theta;
				count++;
			}
		}

		//line merging for left roi
		rho_avg = rho_sum / count;
		theta_avg = theta_sum / count;

		a = cos(theta_avg);
		b = sin(theta_avg);

		x0 = a * rho_avg;
		y0 = b * rho_avg;

		p1.x = cvRound(x0 + (frame.rows + frame.cols) * (-b)) + 200;
		p1.y = cvRound(y0 + (frame.rows + frame.cols) * a) + 400;
		p2.x = cvRound(x0 - (frame.rows + frame.cols) * (-b)) + 200;
		p2.y = cvRound(y0 - (frame.rows + frame.cols) * a) + 400;

		line(frame, p1, p2, Scalar(0, 0, 255), 3, 8);
		
		//reinitalize variables
		rho_sum = 0, theta_sum = 0;
		count = 0;

		//line detection for right roi
		HoughLines(right_roi, lines, 1, CV_PI / 180, 100);

		for (int i = 0; i < lines.size(); i++) {
			rho = lines[i][0];
			theta = lines[i][1];

			//line filtering (angle 120~150 degrees)
			if (theta * (180 / CV_PI) >= 120 && theta * (180 / CV_PI) <= 150) {
				rho_sum += rho;
				theta_sum += theta;
				count++;
			}
		}

		//line merging for right roi
		rho_avg = rho_sum / count;
		theta_avg = theta_sum / count;

		a = cos(theta_avg);
		b = sin(theta_avg);

		x0 = a * rho_avg;
		y0 = b * rho_avg;

		p1.x = cvRound(x0 + (frame.rows + frame.cols) * (-b)) + 600;
		p1.y = cvRound(y0 + (frame.rows + frame.cols) * a) + 400;
		p2.x = cvRound(x0 - (frame.rows + frame.cols) * (-b)) + 600;
		p2.y = cvRound(y0 - (frame.rows + frame.cols) * a) + 400;
		
		line(frame, p1, p2, Scalar(0, 0, 255), 3, 8);
		
		imshow("Frame", frame);

		waitKey(1000 / fps);
	}

	waitKey(0);
	return 0;
}