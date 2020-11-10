#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat frame;
	VideoCapture cap;

	if (cap.open("background.mp4") == 0) {
		cout << "Cannot open video file" << endl;
		waitKey(0);
	}

	double fps = cap.get(CAP_PROP_FPS);

	while (1) {
		cap >> frame;
		if (cap.get(CAP_PROP_POS_MSEC) > 3000) {
			cout << "End of Video" << endl;
			break;
		}
		cout << "frames: " << cap.get(CAP_PROP_POS_FRAMES) << " / " << cap.get(CAP_PROP_FRAME_COUNT) << endl;
		imshow("video", frame);
		waitKey(1000/fps);
	}

	return 0;
}