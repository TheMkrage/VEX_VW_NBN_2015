#include "PID.c"
PID driveLeftPosition, driveRightPosition;
bool driveOnPosition = true;
float driveAngle = 0.0;
int driveEncoderGoal = 0;
int driveAngleGoal = 0;
bool encoderGoalPriority = false;
double x = 0.0;
double y = 0.0;

void resetEncoders() {
	SensorValue[leftEn] = 0;
	nMotorEncoder[leftFront] = 0;
	SensorValue[rightEn] = 0;
	nMotorEncoder[rightFront] = 0;
}

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

float getRightSideMM() {
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

bool isEncoderOnTarget() {
	if (isInRangeOf(getRightSideMM(), driveEncoderGoal, 20) && isInRangeOf(getLeftSideMM(), driveEncoderGoal, 20)) {
		encoderGoalPriority = false;
		return true;
	}
	return false;
}

bool isAngleOnTarget() {
	return isInRangeOf(driveAngle, driveAngleGoal, 3) ;
}

void moveForwardMM(double mm) {
	encoderGoalPriority = true;
	resetEncoders();
	driveEncoderGoal = mm;
}

void initDrive() {
	PIDInit(driveLeftPosition, 2.5, 0.05, 0.5);
	PIDInit(driveRightPosition, 1.0, 0.05, 10.0);
}

task driveNavigator() {
	initDrive();
	while(true) {
		if (!isEncoderOnTarget() && encoderGoalPriority) {
			driveLeftPosition.print = true;
			///setRightSide(80);
			float rightThrottle = PIDRun(driveRightPosition, (float)(driveEncoderGoal - getRightSideMM()));
			float leftThrottle = PIDRun(driveLeftPosition, (float)(getLeftSideMM() - getRightSideMM()));
			setRightSide(rightThrottle);
			setLeftSide(-1 * leftThrottle);
			//writeDebugStreamLine("EN MM right %3.3f left %3.3f", getRightSideMM(), getLeftSideMM());
		}else if (!isAngleOnTarget()) {
			setDrive(0);
		}else {
			setDrive(0);
		}
	}
	wait1Msec(250);
}

task driveTracker() {
	float lastRightRotations = getRightSideRotations();
	float lastLeftRotations = getLeftSideRotations();
	float degreesPerRotations = /*180.0 * (float)((float)ROBOT_WHEEL_DIAMETER_MM/(float)ROBOT_WIDTH_MM)*/ ROBOT_DEG_PER_ROTATION;
	while (true) {
		float rightSide = getRightSideRotations();
		float leftSide = getLeftSideRotations();
		driveAngle += ( (rightSide - lastRightRotations) - (leftSide - lastLeftRotations) )	* degreesPerRotations;
		if ( printEncodersRot ) {
			writeDebugStreamLine("EN ROT right %3.3f left %3.3f", rightSide, leftSide);
		}
		if ( printEncodersMM ) {
			writeDebugStreamLine("EN MM right %3.3f left %3.3f", getRightSideMM(), getLeftSideMM());
		}
		if ( printAngle ) {
			writeDebugStreamLine("ANGLE degPerRot: %3.2f Angle: %3.3f", (rightSide - lastRightRotations), (leftSide - lastLeftRotations), degreesPerRotations, driveAngle);
		}
		lastLeftRotations = leftSide;
		lastRightRotations = rightSide;
		wait1Msec(100);
	}
}
