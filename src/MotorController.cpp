// Created by: Zongyao Jin on 4/24/2017

#include "MotorController.h"
#include "VfsPWM.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "GPIO.h"

using namespace ZJ;

MotorController::MotorController(int in1) {

	pwmControl = new VfsPWM();
	motorAin = new GPIO(in1);
	// std::cout << "Period of PWM: " << pwmControl->getPeriod()  << " ns" << std::endl;

	motorAin->setDirection(GPIO::OUTPUT);
	motorAin->setValue(GPIO::LOW);
	// std::cout << "after set direction" << std::endl;

	this->release();
}

void MotorController::release() {
	motorAin->setValue(GPIO::LOW);
	pwmControl->toggle_pwms(A_PWM,TURN_OFF);
	// std::cout << "after set in1 value" << std::endl;
}

void MotorController::runByVoltage(float voltage) {
	if (voltage >= 0.0f) //move forward
	{
		// this->setForward();
		pwmControl->set_duty_fraction(A_PWM, voltage/MAX_VOLTAGE);
		// pwmControl->toggle_pwms(A_PWM, TURN_ON);
	} 
	else //move backward
	{
		// this->setBackward();
		//invert duty cycle
		pwmControl->set_duty_fraction(A_PWM, 1.0f-(voltage/MAX_VOLTAGE));

	}
}

MotorController::~MotorController() 
{
	delete pwmControl;
}

