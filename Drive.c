#include "PID.c"
PID drivePosition;
bool driveOnPosition = true;
float driveAngle = 0.0;
int driveEncoderGoal = 0;
int driveAngleGoal = 0;

void waitForDrive(int timeout) {
	clearTimer(T4);
	while (!driveOnPosition && time1[T4] < timeout){ wait1Msec(50); }
}

float getLeftSideRotations() {
	return (0.5)*(nMotorEncoder[leftFront]/627.2) + (0.5)*(SensorValue[leftEn]/360.0);
}

float getLeftSideMM() {
	return getLeftSideRotations() * MM_PER_REV;
}

float getRightSideRotations() {
	return (0.5)*(nMotorEncoder[rightFront]/627.2) + (0.5)*(-SensorValue[rightEn]/360.0);
}

float getRighttSideMM() {
	return getRightSideRotations() * MM_PER_REV;
}

void setRightSide(int power) {
	motor[rightFront] = power;
	motor[rightBack] = power;
}

void setLeftSide(int power) {
	motor[leftBack] = power;
	motor[leftFront] = power;
}

void setDrive(int power) {
	setRightSide(power);
	setLeftSide(power);
}


void moveForwardFeedForward(int encoders) {

}

task moveForwardPIDTask() {
	while() {

	}
}

void initDrive() {
	PIDInit(drivePosition, 1, 0, 0);
}

task driveTracker() {
	float lastRightRotations = getRightSideRotations();
	float lastLeftRotations = getLeftSideRotations();
	float degreesPerRotations = 180.0 * (ROBOT_WHEEL_DIAMETER_MM/ROBOT_WIDTH_MM);
	while (true) {
		float rightSide = getRightSideRotations();
		float leftSide = getLeftSideRotations();
		driveAngle += ( (rightSide - lastRightRotations) - (leftSide - lastLeftRotations) )	* degreesPerRotations;
		lastLeftRotations = leftSide;
		lastRightRotations = rightSide;

		writeDebugStreamLine("RightD: %3.3f LeftD: %3.3f defPerRot: %3.2f Angle: %3.3f", (rightSide - lastRightRotations), (leftSide - lastLeftRotations), degreesPerRotations, driveAngle);
			writeDebugStreamLine("right %3.3f last %3.3f", rightSide, lastRightRotations);
		wait1Msec(50);
	}
}
