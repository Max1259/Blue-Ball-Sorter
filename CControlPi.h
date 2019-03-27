#pragma once

#include "pigpio.h"
#include <string>
#include <tuple>

using namespace std;

class CControlPi {

private:

public:

    ~CControlPi() { gpioTerminate(); }

    bool init();

	bool get_data(int type, int channel, int &value);

	bool set_data(int type, int channel, int value);

	int get_button(int channel);

	tuple<float, float> get_analog();

};
