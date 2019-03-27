#include "Blueberry.h"

using namespace cv;
//
//	int iLowH = 0;
//	int iHighH = 179;
//
//	int iLowS = 0;
//	int iHighS = 255;
//
//	int iLowV = 0;
//	int iHighV = 255;

Blueberry::Blueberry() {

    ctrl.init();
    on_off_flag = -1;
    cap.open(0);
}

void Blueberry::update() {
//

//    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
//
//	Create trackbars in "Control" window
//	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
//	cvCreateTrackbar("HighH", "Control", &iHighH, 179);
//
//	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
//	cvCreateTrackbar("HighS", "Control", &iHighS, 255);
//
//	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
//	cvCreateTrackbar("HighV", "Control", &iHighV, 255);


    if (input == 'o') {

        on_off_flag *= -1;
    }

    if (on_off_flag == 1) {

        if (cap.isOpened()) {

            on_off = "Auto";

            //set on LED here
            ctrl.set_data(DIGITAL, 37, 1);

            cap >> frame;
            cap >> blue;
            cap >> others;

            resize(frame, frame, Size(300, 300));

            cvtColor(frame, hsv, CV_BGR2HSV);

            inRange(hsv, Scalar(70, 50, 0), Scalar(100, 255, 255), blue);

            inRange(hsv, Scalar(100, 0, 0), Scalar(179, 255, 255), x);

            inRange(hsv, Scalar(0, 0, 80), Scalar(70, 255, 255), y);

            others = x | y;

            morphologyEx(blue, blue, MORPH_OPEN, Mat(), Point(-1, -1), 1);
            morphologyEx(others, others, MORPH_OPEN, Mat(), Point(-1, -1), 1);
//
            findContours(blue, blue_contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
            findContours(others, other_contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            for (unsigned int i = 0; i < blue_contours.size(); i++) //from notes
            {
                Rect r = boundingRect(blue_contours.at(i));
                if (r.area() > 50000)
                {
                    //cv::rectangle(frame, r, cv::Scalar(255, 255, 255));
                    //drawContours(frame, blue_contours, i, Scalar(255, 255, 255), -1, 8, hierarchy);
                    blue_flag++;
                    break;
                }
            }

            for (unsigned int i = 0; i < other_contours.size(); i++) //From Lecture notes
            {
                Rect r = boundingRect(other_contours.at(i));
                if (r.area() > 50000)
                {
                    //cv::rectangle(frame, r, cv::Scalar(255, 255, 255));
                    //drawContours(frame, other_contours, i, Scalar(255, 255, 255), -1, 8, hierarchy);
                    others_flag++;
                    break;

                }
            }

            //cap >> frame;

            if (blue_flag > 2) {

                //set servo2 to blue pile
                ctrl.set_data(SERVO, 4, 1000);
                //cv::waitKey(100);
                //set green led, turn off red led (pass)
                ctrl.set_data(DIGITAL, 39, 0);
                ctrl.set_data(DIGITAL, 38, 1);

                //set servo1 to release 1 ball
                ctrl.set_data(SERVO, 19, 1800);
                cv::waitKey(100);
                ctrl.set_data(SERVO, 19, 1400);

                pass++;
                std::cout << "pass" << std::endl;

                blue_flag = 0;
                others_flag = 0;
                //cv::waitKey(100);


            } else if (others_flag > 2) {

                //set servo2 to others pile
                ctrl.set_data(SERVO, 4, 1500);
                //cv::waitKey(100);

                //set red led, turn off green led (reject)
                ctrl.set_data(DIGITAL, 38, 0);
                ctrl.set_data(DIGITAL, 39, 1);

                //set servo1 to release 1 ball
                ctrl.set_data(SERVO, 19, 1800);
                cv::waitKey(100);
                ctrl.set_data(SERVO, 19, 1400);

                reject++;
                std::cout << "reject" << std::endl;

                others_flag = 0;
                blue_flag = 0;
                //cv::waitKey(100);
            }

            blue_contours.clear();
            other_contours.clear();
            cv::waitKey(100);

        }

    } else {
        if (cap.isOpened()) {

            //set led off
            on_off = "Manual";
            ctrl.set_data(DIGITAL, 37, 0);

            cap >> frame;
            resize(frame, frame, Size(300, 300));

            if ((ctrl.get_button(32) == 0) || input == 'p') {
                //set servo2 to blue pile
                ctrl.set_data(SERVO, 4, 1000);
                cap >> frame;
                //set green led, turn off red led (pass)
                ctrl.set_data(DIGITAL, 39, 0);
                ctrl.set_data(DIGITAL, 38, 1);

                cap >> frame;

                ctrl.set_data(SERVO, 19, 1800);
                cv::waitKey(100);
                ctrl.set_data(SERVO, 19, 1400);

                pass++;
                std::cout << "pass" << std::endl;

           } else if ((ctrl.get_button(33) == 0) || input == 'r') {
                //set servo2 to others pile
                ctrl.set_data(SERVO, 4, 1500);
                cap >> frame;

                //set red led, turn off green led (reject)
                ctrl.set_data(DIGITAL, 38, 0);
                ctrl.set_data(DIGITAL, 39, 1);

                cap >> frame;

                ctrl.set_data(SERVO, 19, 1800);
                cv::waitKey(100);
                ctrl.set_data(SERVO, 19, 1400);

                reject++;
                std::cout << "reject" << std::endl;
            }
        }
    }
}

void Blueberry::draw() {

//    putText(frame, on_off, Point(5, 20), 2, 1, Scalar::all(255), 1, CV_AA);
//    putText(frame, "Rejected: " + std::to_string(reject), Point(200, 20), 2, 1, Scalar::all(255), 1, CV_AA);
//    putText(frame, "Passed: " + std::to_string(pass), Point(400, 20), 2, 1, Scalar::all(255), 1, CV_AA);
imshow("Bloobs", frame);
//frame = Mat::zeros(frame.size(), CV_8UC3);

}

void Blueberry::run() {

	do {

		update();
		draw();
		input = cv::waitKey(10);

	} while (cv::waitKey(1) != 'q');
}

Blueberry::~Blueberry() {

    ctrl.set_data(SERVO, 19, 1400);
    ctrl.set_data(DIGITAL, 38, 0);
    ctrl.set_data(DIGITAL, 39, 0);
}
