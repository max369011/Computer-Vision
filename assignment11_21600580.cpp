#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image, image_gray, result, background, foregroundMask, element;
	VideoCapture cap;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
	int cnt = 2;
	int num = 0;

	if (cap.open("background.mp4") == 0) {
		cout << "Cannot open video file" << endl;
		waitKey(0);
	}

	double fps = cap.get(CAP_PROP_FPS);
	cap.open("background.mp4");

	//set the first frame as background
	cap >> background;
	cvtColor(background, background, CV_BGR2GRAY);

	while (true) {
		num = 0;
		//play video for 10 seconds
		if (cap.get(CAP_PROP_POS_MSEC) > 10000) break;

		if (cap.grab() == 0) break;
		cap.retrieve(image);

		cvtColor(image, image_gray, CV_BGR2GRAY);

		// current frame(x,y) - background(x,y) = foregroundMask
		absdiff(background, image_gray, foregroundMask);

		//--------------- binary image of foregroundMask -------------------//
		// if (foregroundMask > 50) value = 255 else value = 0
		threshold(foregroundMask, result, 50, 255, CV_THRESH_BINARY);

		//----------------- morphological operation ----------------------//
		element = getStructuringElement(MORPH_ELLIPSE, Size(13, 13));
		dilate(result, result, element);
		morphologyEx(result, result, MORPH_CLOSE, element);

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

		putText(image_gray, format("# of people: %d", num), Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255), 2);

		//imshow("closing", result);
		imshow("Number of people", image_gray);
		//imshow("background", background);
		waitKey(1000 / fps);
	}

	waitKey(0);
	return 0;
}