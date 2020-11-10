#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


Mat drawHistogram(Mat src) {
	Mat hist, histImage, hist2;

	int hist_w, hist_h, bin_w, histSize, histSize2, bin_w2;
	float range[] = { 0,256 };
	const float* histRange = { range };
	
	hist_w = 512;
	hist_h = 512;
	histSize = 16;
	bin_w = cvRound((double)hist_w / histSize);
	
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);
	
	histSize2 = 8;
	bin_w2 = cvRound((double)hist_w / histSize2);
	calcHist(&src, 1, 0, Mat(), hist2, 1, &histSize2, &histRange);
	
	int y = 20;
	//histogram normalization
	for (int j = 0; j < histSize2; j++) {
		float temp = (cvRound(hist2.at<float>(j)) / (float)(src.rows * src.cols));
		putText(src, format("bin %d : %f", j+1, temp), Point(10, y), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);
		y += 15;
	}
		
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 0; i < histSize; i++) {
		rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * i + hist_w / histSize, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1);
	}

	return histImage;
}


int main() {
	Mat moon_img, moon_equalized, hist_equalized_graph, hist_graph, moon_hist;

	moon_img = imread("moon.png", 0);
	if (moon_img.empty()) {
		cout << "No such image file" << endl;
		return -1;
	}
	
	//histogram equalization
	equalizeHist(moon_img, moon_equalized);
	imshow("after", moon_equalized);

	//drawing histogram
	hist_graph = drawHistogram(moon_img);
	cout << endl;
	hist_equalized_graph = drawHistogram(moon_equalized);

	//display images
	imshow("h1", hist_graph);
	imshow("h2", hist_equalized_graph);

	imshow("before", moon_img);
	imshow("after", moon_equalized);
	
	waitKey(0);
	return 0;
}