/* Creaded by: Zongyao Jin
 *
 */

#ifndef EQEP_H_
#define EQEP_H_
#include<string>
using std::string;


#define EQEP_A "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/"
#define EQEP_B "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/"
#define EQEP_POSITION "position"
#define EQEP_ENABLE "enable"
#define COUNTS_PER_REV 1632.67f
#define PI 3.14159265358979323846f

namespace ZJ {

class EQEP {

private:
	string path;
	int position;
	float rad, degree;

public:
	EQEP(bool i);

	virtual int setZero();
	virtual int getPosition();
	virtual void updateSensor();
	virtual float getRad();
	virtual float getDeg();

	virtual int run();
	virtual bool isRunning();
	virtual int stop();

	virtual ~EQEP();

};

} /* namespace ZJ */

#endif /* EQEP_H_ */
