/* Main Loop for BalanceBotBlack */
/* Encoders */
	#include "eqep.h"
	#include "util.h"
	#include <cstdlib>
/* IMU */
	#include "MPU9250.h"
	#include <iostream>
	#include <unistd.h>
	#include <cmath>
	#include <stdio.h>
	#include <iomanip>
/* PWM */
	#include "PWM.h"
/* Motor Driver */
	#include "MotorController.h"
	#include "GPIO.h"
	#include <string>
/* Timing */
	#include <chrono>
	#include <fstream>
	#include <sys/time.h>
/* Matrix Math */
	#include <Eigen/Dense>
	
	
	using namespace ZJ;
	using namespace std;
	using Eigen::MatrixXf;

/* Timing */
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::nanoseconds ns;
    typedef std::chrono::duration<float> fsec;
	
int main(){
// Initialize everything outside the loop
	cout << "Start\n";
	
/* Initialize the IMU */
	MPU9250 imu(2, 0x68);
    imu.setAccRange(MPU9250::PLUSMINUS_16_G);
    imu.setGyroRange(MPU9250::PLUSMINUS_2000_DPS);
	
/* Loop Variables */
	bool killSwitch = 1;
	unsigned long i = 0;
	float last1 = 0;
	float last2 = 0;
	float last3 = 0;
	float last4 = 0;
	float lastAvg = 0;
	float imuAvg = 0;
	
	float lastAcc1 = 0;
	float lastAcc2 = 0;
	float lastAcc3 = 0;
	float lastAcc4 = 0;
	float accAvg = 0;
	
/* Encoders */
	EQEP encoder1(0);	//P8_35, P8_33
	int pos1 = 0;
	EQEP encoder2(1);	//P8_12, P8_11
	int pos2 = 0;
	
/* PWM */
	PWM motor1("pwm1a"); //P9_14
	PWM motor2("pwm1b"); //P9_16
	GPIO gip1(30); //P9_11
	GPIO gip2(31); //P9_13
	
/* Control Variables */
	float volt1 = 0.0;
	float volt2 = 0.0;
	float lastE1 = 0;
	float e1 = 0.0;
	float pos_d = 0.0;
	float e_pos = 0.0;
	float torque_to_percent = 210; //210 is what it's supposed to be
	float count_to_radians = 2*PI/1632;
	float R = 0.03; // meters
	float control1 = 0.0;
	float control2 = 0.0;
	float alpha = 0.98;
	
	float sampTime = 0.010; //0.005
	float totalLoopTime = 0.02;
	
	float AccE = 0.0; 
	float AccY = 0.0;
	float GyroE = 0.0;
	float GyroY = 0;
	float phi = 0.0;
	int counter = 1;
	float angleOffset = 0.0;
	
	float kp = 100;//92/5*180/PI; 100
	// float kd = 92/(5/sampTime)*180/PI/2;
	float kd = 30;
	float y1 = 0;
	
/* H-Control Matrices */
	MatrixXf A(3,3); // use MatrixXf for arbitrary size
	A <<  0.8406	,	-0.108	,	-0.5904	,
	-0.02299	,	0.9754	,	-0.1062	,
	-0.06604	,	-0.04553,	0.6902	;
	
	 
	MatrixXf B(3,2);
	B << -0.001162	,	-0.002217	,
	0.004662	,	-0.0003631	,
	-0.0009133	,	-0.001294	;
		 
	MatrixXf C(1,3);
	C <<1.038	,	0.7065	,	1.966	;
	

	MatrixXf D(1,2);
	D << 0.00003297	,	0.02139	;

	
	MatrixXf q(3,1);
	q << 0,0,0;
	MatrixXf u(2,1);
	u << 0,0;
	MatrixXf y(1,1);
	y << 0;
	//Vector2d q(0,0); // column vectors by default
	//Vector2d u(0,0);	
	//Vector2d y(0,0);
	
	auto ta = Time::now();
	auto t0 = Time::now();
	auto t1 = Time::now();
	auto t2 = Time::now();
	auto t3 = Time::now();
	auto t4 = Time::now();
	auto t5 = Time::now();
	auto t6 = Time::now();
	auto t7 = Time::now();
	auto t8 = Time::now();
	auto t9 = Time::now();
	
	
///* THE LOOP OF ETERNAL BALANCE *///
	while (killSwitch){
	// ta = Time::now();
	// t0 = Time::now();
	/* Read Encoders */
		pos1 = encoder1.getPosition();
		pos2 = encoder2.getPosition();
		
		
	/* Read IMU */
		// t1 = Time::now();
		imu.readSensor();
		// t2 = Time::now();
		/* use Felipe's kalman filter here */
		
		
	/* Calculate Control Effort */
		//e1 = imu.getAccPitch(); // stall voltage is 1V
		//control1 = 92*(1/10.0)*e1 + 8; //percent duty cycle
		//control2 = 92*(1/10.0)*e1 + 8;
	
	
	/* Calculate H-Control Effort */
	last4 = last3;
	last3 = last2;
	last2 = last1;
	last1 = e1;
	
	lastAcc4 = lastAcc3;
	lastAcc3 = lastAcc2;
	lastAcc2 = lastAcc1;
	lastAcc1 = AccE;
	// t3 = Time::now();
	
	
	
		AccE = imu.getAccPitch();
		// AccY = imu.getAccRoll();
		GyroY = imu.getGyroOmegaY()*sampTime;
		GyroE = phi + GyroY;
		phi = (1-alpha)*AccE + alpha*GyroE;
		lastAvg = imuAvg;
		e1 = (phi-angleOffset)*PI/180;
	// t4 = Time::now();
	
		imuAvg = (e1+last1+last2+last3+last4)/5.0;
		accAvg = (AccE+lastAcc1+lastAcc2+lastAcc3+lastAcc4)/5.0;
		
		e_pos = (pos1 - pos_d)*count_to_radians*R; //possible sign error
		u(1) = imuAvg;
		u(0) = e_pos;
	// t5 = Time::now();
		// y = C*q + D*u;	//   y[k] = C*q[k] + D*u
		// q = A*q + B*u;	// q[k+1] = A*q[k] + B*u
	// t6 = Time::now();
		
		// control1 = abs(y(0))*torque_to_percent + 8.0; //torque_to_percent is 92%/maxtorque
		// control2 = abs(y(0))*torque_to_percent + 8.0;
		// y1 = y(0);
		y1 = kp*imuAvg + kd*(imuAvg-lastAvg)/sampTime;
		// y1 = -kp*imu.getGyroOmegaZ()*sampTime;
		
		control1 = abs(y1) + 8;
		control2 = control1;
		
		/* Prevent Saturation */
			if (abs(imuAvg) >= 60*PI/180){
				killSwitch = 0;
				control1 = 0;
				control2 = 0;
			}
			
			/* if (abs(imuAvg) >= 10*PI/180){
				
				kp = 100;
				kd = 750;
			}
			else {
				kp = 25.0;
				kd = 25.0;
			} */
			
		if (control1 > 100.0){

			control1 = 100.0;
			control2 = 100.0;
		}
		
		// t7 = Time::now();
	/* Output Pwm */
		if (y1 >= 0){
			gip1.setValue(GPIO::LOW);
			gip2.setValue(GPIO::LOW);		
			
			motor1.setDutyCyclePercentage(control1);
			motor2.setDutyCyclePercentage(control2);
		}
		else {
			gip1.setValue(GPIO::HIGH);
			gip2.setValue(GPIO::HIGH);
			
			motor1.setDutyCyclePercentage(100 - control1);
			motor2.setDutyCyclePercentage(100 - control2);
		}
		
		//cout << imu.getAccPitch() << "		" << control1 << "		" << control2 << "		" << e1<<"\n";
		// t8 = Time::now();
		// t9 = Time::now();
		
		
		// fsec fs = t1 - t0;
		// fsec fIMU = tIMU2 - tIMU1;
		//cout << fs.count() << "\n";
		cout  << "Avg: " << imuAvg*180/PI << "		Angle: " << e1*180/PI << "		C1: " << control1 << "		Y: " << y1 <<"		AccPitch: " << AccE <<"		GyroY: " << GyroY <<"\n";
		// cout <<"ANGLE:	"<< imuAvg*180/PI<<"	IMU Time:	"<<fIMU.count()<<"\n";
		// if (counter == 100){
		// break;
		// }
		usleep(totalLoopTime-sampTime);
		// counter = counter + 1;
		
	 }
		fsec fta = t0-t0;
	 	fsec ft0 = t0-ta;
		fsec ft1 = t1-t0;
		fsec ft2 = t2-t1;
		fsec ft3 = t3-t2;
		fsec ft4 = t4-t3;
		fsec ft5 = t5-t4;
		fsec ft6 = t6-t5;
		fsec ft7 = t7-t6;
		fsec ft8 = t8-t7;
		fsec ft9 = t9-ta;
		
		cout << "T0: "<< ft0.count() << "		T1: " << ft1.count() << "		T2: " << ft2.count()<< "		T3: " << ft3.count()<< "		T4: " << ft4.count()<< "		T5: " << ft5.count()<< "		T6: " << ft6.count()<< "		T7: " << ft7.count()<< "		T8: " << ft8.count()<< "		T9: " << ft9.count() <<"\n";
		

////////////////////* Chrono Sample Code *////////////////////
    // auto t0 = Time::now();
    // auto t1 = Time::now();
    // fsec fs = t1 - t0;
    // ns d = std::chrono::duration_cast<ns>(fs);
    // std::cout << fs.count() << " s\n";
    // std::cout << d.count() << " ms\n";
    // std::cout << MotorEnCnt/d.count() << " ms\n";
//////////////////////////////////////////////////////////////


////////////////* Getting Motor Parameters *//////////////////
	// ofstream out_data;
	// out_data.open("encoder_rate.dat");
	// motor.runByVoltage(12.0);
	// int iter = 0;
    // auto t1 = Time::now();
	// int pos_now = 0;
	// double encoder_rate = 0;
	// usleep(10000000);
	// motor.runByVoltage(0.0);
	// int pos_prev = encoder2.getPosition();
    // auto t0 = Time::now();
	// auto current_time = Time::now();
	// auto start_time = Time::now();
	
	// while (iter < 1000){
		
		// pos_now = encoder2.getPosition();
		
		// t1 = Time::now();
		// fsec fs = t1 - t0;
		// ns d = std::chrono::duration_cast<ns>(fs);

		// encoder_rate = (double)1000*(pos_now - pos_prev)/d.count(); // counts per second
		
		// out_data << fs.count() <<"	" << encoder_rate << "\n";
		// cout << encoder_rate << "	" << pos_now << "	" << pos_prev <<"\n";
		// t0 = t1;
		
		// if (pos_prev == pos_now){
			// ++iter;
		// }
		// else{
			// iter = 0;
		// }
		// pos_prev = pos_now;
		
	// }
	// out_data.close();
//////////////////////////////////////////////////////////////
	
	return 0;
}
