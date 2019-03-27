// Caressed softly by Christian DeBuys
// Modified by Felipe Constantin Reyes-Miftajov
// Based on L298N.h and .cpp
// Created by: Zongyao Jin on 4/24/2017

// Uses one PWM for forward direction for two motors, one for reverse.
// Enable pins are always set high.

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include "VfsPWM.h"
#include "GPIO.h"
#include <cstdlib>

#define MAX_VOLTAGE 12.0f

enum PWMselect
	{
		A_PWM = 0,
		B_PWM = 1
	};

class MotorController {

public:

    MotorController(int in1, int in3);
	~MotorController();
	void release();
	void runByVoltage(float voltage);
	void setForward();
	void setBackward();
	
private:

	VfsPWM* pwmControl;
	ZJ::GPIO* motorAin;
	ZJ::GPIO* motorBin;
		
	void setCW();
	void setCCW();

};

#endif
