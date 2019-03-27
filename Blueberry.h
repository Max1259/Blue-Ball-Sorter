#pragma once

#include <opencv2/opencv.hpp>
#include "CControlPi.h"

#include <vector>
#include <string>

using namespace cv;

class Blueberry {
private:

public:

    enum { DIGITAL = 0, ANALOG, SERVO };

    Mat hsv, blue, others, x, y, frame;

    CControlPi ctrl;

    int on_off_flag;
    int blue_flag = 0;
    int others_flag = 0;
    int pass = 0;
    int reject = 0;

    string on_off = "";

	vector<Vec4i> hierarchy;
	vector<vector<Point>> blue_contours;
	vector<vector<Point>> other_contours;

	VideoCapture cap;

	char input = 's';

    Blueberry();
    ~Blueberry();
    void update();
    void draw();
    void run();

};
