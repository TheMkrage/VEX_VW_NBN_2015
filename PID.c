#include "PID.h"
// safety
#ifndef PID_SOURCE
#define PID_SOURCE
#pragma systemFile
void PIDInit(PID controller, float kP, float kD) {
	controller.kP = kP;
	controller.kI = 0;
	controller.kD = kD;
	controller.integral = 0;
	controller.kILimit = 50;
	controller.print = false;
}

void PIDInit(PID controller, float kP, float kI, float kD) {
	controller.kP = kP;
	controller.kI = kI;
	controller.kD = kD;
	controller.integral = 0.0;
	controller.kILimit = 50.0;
	controller.print = false;
}

float PIDRun(PID controller, float error) {

	// calculate the derivative
	float pidDerivativ =  error - controller.previousError;

	// add integral
	controller.integral += error * controller.kI;
	if(controller.integral > controller.kILimit)
		controller.integral = controller.kILimit;

	if(controller.integral < -controller.kILimit)
		controller.integral = -controller.kILimit;

	if ( controller.print ) {
		writeDebugStreamLine("\n P: %3.3f I: %3.3f D: %3.3f \n error: %3.3f outPut: %3.3f", (float)(controller.kP * error), (float)( controller.integral), (float)(controller.kD * pidDerivativ), error, (float)((float)(controller.kP * error)  + (float)(controller.kI * controller.integral) + (float)(controller.kD * pidDerivativ)));
	}
	controller.previousError = (float)error;

	// return kP * error + kI * integral + kD * derivative
	return (float)((float)(controller.kP * error)  + (float)(controller.integral) + (float)(controller.kD * pidDerivativ));
}

// Sets the limit for the integral constant
void PIDSetIntegralLimit(PID controller, float kILimit) {
	controller.kILimit = kILimit;
}

// Restes the integral value
void PIDResetIntegral(PID controller) {
	controller.integral = 0;
}

#endif
