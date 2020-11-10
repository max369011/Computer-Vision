#include "cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {

	Mat lena_img, lena_clone, bgr_channels[3], hsv_channels[3];
	Mat colorful_img, colorful_clone;
	Mat balancing_img, balancing_clone, balancing_roi;
	uchar* h;
	uchar* s;
	uchar* v;

	lena_img = imread("lena.png");
	colorful_img = imread("colorful.jpg");
	balancing_img = imread("balancing.jpg");

	if (lena_img.empty() || colorful_img.empty() || balancing_img.empty()) {
		cout << "Could not open the image files" << endl;
		return -1;
	}

	imshow("lena", lena_img);
	imshow("colorful", colorful_img);
	imshow("balancing", balancing_img);

	balancing_clone = balancing_img.clone();
	colorful_clone = colorful_img.clone();
	lena_clone = lena_img.clone();
	
	while (1) {
		int key = waitKey(0);

		if (key == 27) {
			break;
		}
		else if (key == 110) {	//if key = 'n' perform negative transformation

			split(lena_clone, bgr_channels);
			for (int i = 0; i < 3; i++) {
				bgr_channels[i] = 255 - bgr_channels[i];
			}
			merge(bgr_channels, 3, lena_clone);
			imshow("lena", lena_clone);

		}
		else if (key == 103) {	// if key = 'g' perform gamma transformation
			double gamma = 2.5;
			//double consnt = 1.0;
			lena_clone.convertTo(lena_clone, CV_32F);
			normalize(lena_clone, lena_clone, 0, 1.0, NORM_MINMAX);
			pow(lena_clone, gamma, lena_clone);
			//lena_gamma = consnt * lena_gamma;
			normalize(lena_clone, lena_clone, 0, 255, NORM_MINMAX);
			convertScaleAbs(lena_clone, lena_clone);
			imshow("lena", lena_clone);

		}
		else if (key == 104) {	//if key = 'h' perform histogram equalization 
			cvtColor(lena_clone, lena_clone, CV_BGR2HSV);
			split(lena_clone, hsv_channels);
			equalizeHist(hsv_channels[2], hsv_channels[2]);
			merge(hsv_channels, 3, lena_clone);
			cvtColor(lena_clone, lena_clone, CV_HSV2BGR);
			imshow("lena", lena_clone);
		}
		else if (key == 115) {	//if key = 's' perform color slicing image (9<hue<23)
			cvtColor(colorful_clone, colorful_clone, CV_BGR2HSV);
			split(colorful_clone, hsv_channels);

			for (int i = 0; i < colorful_clone.rows; i++) {
				h = hsv_channels[0].ptr<uchar>(i);
				s = hsv_channels[1].ptr<uchar>(i);
				for (int j = 0; j < colorful_clone.cols; j++) {
					if (h[j] > 9 && h[j] < 23)
						s[j] = s[j];
					else
						s[j] = 0;
				}
			}

			merge(hsv_channels, 3, colorful_clone);
			cvtColor(colorful_clone, colorful_clone, CV_HSV2BGR);
			imshow("colorful", colorful_clone);
		}
		else if (key == 99) {	//if key = 'c' perform color conversion (hue +50)
			cvtColor(colorful_clone, colorful_clone, CV_BGR2HSV);
			split(colorful_clone, hsv_channels);

			for (int i = 0; i < colorful_clone.rows; i++) {
				h = hsv_channels[0].ptr<uchar>(i);
				s = hsv_channels[1].ptr<uchar>(i);
				for (int j = 0; j < colorful_clone.cols; j++) {
					if (h[j] < 129)
						h[j] += 50;
					else
						h[j] -= 129;
				}
			}

			merge(hsv_channels, 3, colorful_clone);
			cvtColor(colorful_clone, colorful_clone, CV_HSV2BGR);
			imshow("colorful", colorful_clone);
		}
		else if (key == 97) {	//if key ='a' perform average filtering

			Rect rect(0, 0, balancing_clone.size().width, balancing_clone.size().height);
			balancing_roi = balancing_clone(rect);
			blur(balancing_roi, balancing_roi, Size(9, 9));
			balancing_roi.copyTo(balancing_clone);

			imshow("balancing", balancing_clone);
		}
		else if (key == 119) {	//if key = 'w' perform clone balancing
			int b_sum = 0, g_sum = 0, r_sum = 0;
			uchar* value_B;
			uchar* value_G;
			uchar* value_R;
			int b_avg, g_avg, r_avg, b_gray, g_gray, r_gray;
			int totalPix = balancing_clone.rows * balancing_clone.cols;

			split(balancing_clone, bgr_channels);

			for (int i = 0; i < balancing_clone.rows; i++) {
				value_B = bgr_channels[0].ptr<uchar>(i);
				value_G = bgr_channels[1].ptr<uchar>(i);
				value_R = bgr_channels[2].ptr<uchar>(i);
				for (int j = 0; j < balancing_clone.cols; j++) {
					b_sum += value_B[j];
					g_sum += value_G[j];
					r_sum += value_R[j];
				}
			}

			b_avg = b_sum / totalPix;
			g_avg = g_sum / totalPix;
			r_avg = r_sum / totalPix;

			// color balancing using gray world assumsption
			for (int i = 0; i < balancing_clone.rows; i++) {
				value_B = bgr_channels[0].ptr<uchar>(i);
				value_G = bgr_channels[1].ptr<uchar>(i);
				value_R = bgr_channels[2].ptr<uchar>(i);
				for (int j = 0; j < balancing_clone.cols; j++) {

					b_gray = (128 * value_B[j]) / b_avg;
					g_gray = (128 * value_G[j]) / g_avg;
					r_gray = (128 * value_R[j]) / r_avg;

					// if b_gray is over 255
					if (b_gray > 255) {
						value_B[j] = 255;
					}
					else {
						value_B[j] = b_gray;
					}

					//if g_gray is over 255 
					if (g_gray > 255) {
						value_G[j] = 255;
					}
					else {
						value_G[j] = g_gray;
					}

					//if r_gray is over 255
					if (r_gray > 255) {
						value_R[j] = 255;
					}
					else {
						value_R[j] = r_gray;
					}
				}
			}

			merge(bgr_channels, 3, balancing_clone);
			imshow("balancing", balancing_clone);
		}
		else if (key == 114) {	//if key = 'r' reset images
			balancing_clone = balancing_img.clone();
			lena_clone = lena_img.clone();
			colorful_clone = colorful_img.clone();


			imshow("colorful", colorful_img);
			imshow("lena", lena_img);
			imshow("balancing", balancing_img);
		}
	}

	return 0;
}