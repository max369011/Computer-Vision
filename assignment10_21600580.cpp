#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat frame, image, image_gray, avg, result, background, foregroundMask, foregroundImg;
	VideoCapture cap;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	int cnt = 2;
	int num = 0;

	if (cap.open("background.mp4") == 0) {
		cout << "Cannot open video file" << endl;
		waitKey(0);
	}

	double fps = cap.get(CAP_PROP_FPS);
	cap.open("background.mp4");
	

	cap >> frame;
	cvtColor(frame, frame, CV_BGR2GRAY);

	while (true) {
		num = 0;
		if (!cap.read(image)) break;

		cvtColor(image, image_gray, CV_BGR2GRAY);

		// avg background for the first 10 frames
		if (cap.get(CAP_PROP_POS_FRAMES) <= 10) {
			add(image_gray / cnt, frame * (cnt-1) / cnt, background);
			cnt++;
		}

		// current frame(x,y) - background(x,y) = foregroundMask
		absdiff(image_gray, background, foregroundMask);

		//--------------- binary image of foregroundMask -------------------//
		// if (foregroundMask > 50) value = 255 else value = 0
		threshold(foregroundMask, result, 20, 255, CV_THRESH_BINARY);


		//------------- drawing bounding rectangle of objects -------------//
		//threshold(result, result, 20, 255, CV_THRESH_BINARY);
		findContours(result, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		

		//------------- defining and drawing rectangles on contours -------------//
		//defining bounding rectangle
		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++)
			boundRect[i] = boundingRect(Mat(contours[i]));
		
		//draw rectangles on the contours
		for (int i = 0; i < contours.size(); i++) {
			if (boundRect[i].area() > 400) {
				rectangle(image_gray, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 255), 2, 8, 0);
				num++;
			}
		}

		putText(image_gray, format("# Rect: %d", num), Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255), 4);


		//imshow("result", result);
		imshow("background subtraction", image_gray);
		//imshow("background", background);
		waitKey(1000 / fps);
	}

	waitKey(0);
	return 0;
}