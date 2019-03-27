/*
* Pi connections to boosterpack:
*
* Type:     Booster:    Pi:
* -------------------------
* RED       39          15
* GREEN     38          13
* BLUE      37          11
* 5V        21          2,4
* 3.3V      1           1
* PUSH1     33          16
* PUSH2     32          18
* PUSH3     5           33
* GND       22          6,9
* SERVO1    19          12
* SERVO2    4           22
*
* ADC:
* 16, 15    ->  3.3V
* 14, 9     ->  GND
* 1         ->  Boost26 (Y)
* 2         ->  Boost2  (X)
* 13        ->  SCLK    (PI23)
* 12        ->  MISO    (PI21)
* 11        ->  MOSI    (PI19)
* 10        ->  CS0     (PI24)
*
*/

#include "CControlPi.h"
#include "pigpio.h"
#include <tuple>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

#define MAX_X 1015
#define MAX_Y 1024
#define MIN_X 10
#define MIN_Y 20

#define BLU 17
#define GRN 27
#define RED 22

#define PUSH1 23
#define PUSH2 24
#define PUSH3 13

#define SERV1 18
#define SERV2 25

int button_count = 0;

using namespace std;

enum { DIGITAL = 0, ANALOG, SERVO };

bool CControlPi::init() {

    if (gpioInitialise() < 0) {

        return 1;
    }

    gpioSetMode(BLU, PI_OUTPUT);
    gpioSetMode(GRN, PI_OUTPUT);
    gpioSetMode(RED, PI_OUTPUT);

    gpioSetMode(PUSH1, PI_INPUT);
    gpioSetMode(PUSH2, PI_INPUT);
    gpioSetMode(PUSH3, PI_INPUT);

    gpioSetMode(SERVO, PI_OUTPUT);

}

bool CControlPi::get_data(int type, int channel, int &value) {

    int c, v;

        if (type == DIGITAL) {

            switch (channel) {
                case 37:
                    c = BLU;
                    break;

                case 38:
                    c = GRN;
                    break;

                case 39:

                    c = RED;
                    break;

                case 33:
                    c = PUSH1;
                    break;

                case 32:
                    c = PUSH2;
                    break;

                case 5:
                    c = PUSH3;
                    break;
            }
        }


        v = gpioRead(c);
        value = v;
        //std::cout << value << std::endl;

    return true;
}

bool CControlPi::set_data(int type, int channel, int value) {

    int c;

    if (type == DIGITAL) {
        if (channel == 37) {
            c = BLU;
        }
        if (channel == 38) {
            c = GRN;
        }
        if (channel == 39) {
            c = RED;
        }
        gpioWrite(c, value);

    } else if (type == SERVO) {
        if (channel == 19) {

            c = SERV1;

        } else if (channel == 4) {

            c = SERV2;
        }

        gpioServo(c, value);
    }

    return true;
}

int CControlPi::get_button(int channel) {

    int val;
    get_data(DIGITAL, channel, val);
    return val;
}

tuple<float, float> CControlPi::get_analog() {

    gpioInitialise();

    float analog_percent_x, analog_percent_y;
    int value;
    float read_x, read_y;
    unsigned char inBuf[3];

    while(1) {

        int handle = spiOpen(0, 200000, 3);

        char cmdy[] = { 1, 128, 0 };

        spiXfer(handle, cmdy, (char*) inBuf, 3);
        read_y = ((inBuf[1] & 3) << 8) | inBuf[2];

        spiClose(handle);

        handle = spiOpen(0, 200000, 3);

        char cmdx[] = { 1, 144, 0 };

        spiXfer(handle, cmdx, (char*) inBuf, 3);
        read_x = ((inBuf[1] & 3) << 8) | inBuf[2];

        spiClose(handle);

        analog_percent_y = ((MAX_Y - read_y + MIN_Y) / (MAX_Y - MIN_Y));
        cout << setprecision(2) << fixed << "ANALOG TEST Y: " << read_y << " (" << analog_percent_y << ")  ";

        analog_percent_x = ((read_x - MIN_X) / (MAX_X - MIN_X));
        cout << setprecision(2) << fixed << setw(20) << "ANALOG TEST X: " << read_x << " (" << analog_percent_x << ")" << endl;

        return make_tuple(analog_percent_x, analog_percent_y);
    }
}


