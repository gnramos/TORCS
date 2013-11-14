
/** @file PIDDriver.cpp
  *
  * Defines the PIDDriver.
  *
  * @author Guilherme N. Ramos (gnramos@unb.br)
  */

#include <math.h>

#include "CarControl.h"
#include "CarState.h"
#include "PIDDriver.h"

int
getGear(CarState & cs) {
	int current_gear = cs.getGear();
	if(!current_gear) return 1;

	if(cs.getRpm() > 8000) ++current_gear;
	else if(current_gear > 1 && cs.getRpm() < 5000) --current_gear;
	return current_gear;
}

float
getSteering(CarState & cs) {
	// based on Loiacono's SimpleDriver

	const float
	  steerLock = 0.366519;
	float
	  targetAngle = (cs.getAngle() - cs.getTrackPos() * 0.5) / steerLock;

	// normalize steering
	if(targetAngle < -1)
		targetAngle = -1;
	else if(targetAngle > 1)
		targetAngle = 1;

	return targetAngle;
}


float
getSpeed(CarState & cs) {
	return sqrt(pow(cs.getSpeedX(), 2) + pow(cs.getSpeedY(), 2));
}


PIDDriver::PIDDriver():BaseDriver(), speedPID(KP, KI, KD)
{
};

PIDDriver::~PIDDriver()
{
};

string
PIDDriver::drive(string sensors) {
	CarState cs(sensors);

	float brake = 0, steer = getSteering(cs), clutch = 0;

	float accel = speedPID.output(finalSpeed, getSpeed(cs), PID_DT);

	int gear = getGear(cs), focus = 0, meta = 0;

	CarControl cc(accel, brake, gear, steer, clutch, focus, meta);

	return cc.toString();
}
