// Created by: Zongyao Jin on 4/24/2017

#include "L298N.h"
#include "GPIO.h"
#include "PWM.h"
#include <cstdlib>
#include <string>
#include <iostream>
using std::string;

namespace ZJ {

	L298N::L298N(string pwm, int in1, int in2) {

		pwmControl = new PWM(pwm);
		in1Control = new GPIO(in1);
		in2Control = new GPIO(in2);
		// std::cout << "Period of PWM: " << pwmControl->getPeriod()  << " ns" << std::endl;

		in1Control->setDirection(GPIO::OUTPUT);
		in1Control->setValue(GPIO::LOW);
		// std::cout << "after set direction" << std::endl;
		in2Control->setDirection(GPIO::OUTPUT);
		in2Control->setValue(GPIO::LOW);

		this->release();
	}

	void L298N::release() {
		in1Control->setValue(GPIO::LOW);
		// std::cout << "after set in1 value" << std::endl;
		in2Control->setValue(GPIO::HIGH);
		pwmControl->setDutyCyclePercentage(0);
	}

	void L298N::setCW() {
		in1Control->setValue(GPIO::HIGH);
		in2Control->setValue(GPIO::LOW);
	}

	void L298N::setCCW() {
		in1Control->setValue(GPIO::LOW);
		in2Control->setValue(GPIO::HIGH);
	}

	void L298N::runByVoltage(float voltage) {
		if (voltage >= 0.0f) {
			this->setCW();
			if (voltage <= 12.0f) {
				pwmControl->setDutyCyclePercentage(voltage/12.0f*100.0f);
				pwmControl->run();
			} else {
				pwmControl->setDutyCyclePercentage(100.0f); // control input saturation
				pwmControl->run();
			}
		} else {
			this->setCCW();
			if (voltage >= -12.0f) {
				pwmControl->setDutyCyclePercentage(-voltage/12.0f*100.0f);
				pwmControl->run();
			} else {
				pwmControl->setDutyCyclePercentage(100.0f); // control input saturation
				pwmControl->run();
			}
		}
	}

	L298N::~L298N() {}

}

