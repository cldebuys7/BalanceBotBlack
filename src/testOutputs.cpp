// Main Loop for Balance Bot Black
/* Encoders */
	#include "eqep.h"
	#include "util.h"
	#include <cstdlib>
/* IMU */
	#include "MPU9250.h"
	#include <iostream>
	#include <unistd.h>
	#include <math.h>
	#include <stdio.h>
	#include <iomanip>
/* PWM */
	#include "PWM.h"
	//#include "util.h"
	#include <cstdlib>
	//#include <iostream>
/* Motor Driver */
	#include "MotorController.h"
	#include "GPIO.h"
	//#include "PWM.h"
	//#include <cstdlib>
	#include <string>
	//#include <iostream>
/* Timing */
	#include <chrono>
	#include <fstream>
	#include <sys/time.h>

using namespace ZJ;
using namespace std;

// typedef std::chrono::high_resolution_clock Clock;
// typedef std::chrono::nanoseconds ns;
// typedef std::chrono::duration<float> fsec;
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::nanoseconds ns;
    typedef std::chrono::duration<float> fsec;
	
int main(){
	cout << "Start\n";
	
	MotorController motor1(50); //P9_14
	MotorController motor2(51); //P9_16
	GPIO gip1(30); //P9_11
	GPIO gip2(31); //P9_13

	motor1.runByVoltage(12.0);
	motor2.runByVoltage(12.0);
	gip1.setValue(GPIO::HIGH);
	gip2.setValue(GPIO::HIGH);
	
	return 0;
}

