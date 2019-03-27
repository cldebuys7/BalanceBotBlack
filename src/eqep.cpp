/* Created by: Zongyao Jin
 *
 */

#include "eqep.h"
#include "util.h"
#include <iostream>
#include <cstdlib>

namespace ZJ {

EQEP::EQEP(bool i) {

	if (i == 0) this->path = EQEP_A;
	else this->path = EQEP_B;

	this->setZero();
	this->position = this->getPosition();

}

int EQEP::setZero(){
	return write(this->path, EQEP_POSITION, 0);
}

int EQEP::getPosition(){
	return atoi(read(this->path, EQEP_POSITION).c_str());
}

void EQEP::updateSensor() {
	position = this->getPosition();
}

float EQEP::getRad(){
	this->updateSensor();
	return (position/COUNTS_PER_REV * 2.0f * PI);
}

float EQEP::getDeg(){
	this->updateSensor();
	return (position/COUNTS_PER_REV * 360.0f);
}


int EQEP::run(){
	// It seems that if the DTO is deployed, enable is always 1.
	// And we don't have permission to enable it again.
	return write(this->path, EQEP_ENABLE, 1);
}

bool EQEP::isRunning(){
	string running = read(this->path, EQEP_ENABLE);
	return (running=="1");
}

int EQEP::stop(){
	return write(this->path, EQEP_ENABLE, 0);
}

EQEP::~EQEP() {}

} /* namespace ZJ */
