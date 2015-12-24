#include "PID.c"
PID driveLeftPosition, driveRightPosition, anglePosition;
bool driveOnPosition = true;
float driveAngle = 0.0;
int driveEncoderGoal = 0;
int driveAngleGoal = 0;
bool encoderGoalPriority = false, angleGoalPriority = false;
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
	while (!driveOnPosition ){
		wait1Msec(150);
	}
	encoderGoalPriority = false;
	angleGoalPriority = false;
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
	if (isInRangeOf(getRightSideMM(), driveEncoderGoal, 10)) {
		encoderGoalPriority = false;
		return true;
	}
	return false;
}

bool isAngleOnTarget() {
	if (isInRangeOf(driveAngle, driveAngleGoal, 3) ) {
		angleGoalPriority = false;
		return true;
	}
	return false;
}

void moveForwardMM(double mm) {
	encoderGoalPriority = true;
	resetEncoders();
	driveEncoderGoal = mm;
	wait1Msec(500);
}

void addToAngle(double angleAdd) {
	angleGoalPriority = true;
	driveAngleGoal = driveAngle - angleAdd;
	wait1Msec(500);
}

void initDrive() {
	PIDInit(driveLeftPosition, 3.0, 0.0, 10.0);
	PIDInit(driveRightPosition, 3.0, 0.00, 10.0);
	PIDInit(anglePosition, 4.1, 0, 2.5);
}

task driveNavigator() {
	initDrive();
	while(true) {
		if (!isEncoderOnTarget() && encoderGoalPriority) {
			driveOnPosition = false;
			//driveRightPosition.print = true;
			driveLeftPosition.print = true;
			float rightThrottle = PIDRun(driveRightPosition, (float)(driveEncoderGoal - getRightSideMM()));
			float leftThrottle = PIDRun(driveLeftPosition, (float)(driveEncoderGoal - getRightSideMM()));
			setRightSide(rightThrottle);
			setLeftSide(leftThrottle);
		}else if (!isAngleOnTarget() && angleGoalPriority) {
			driveOnPosition = false;
			float turn = PIDRun(anglePosition, (float)(driveAngleGoal - driveAngle));
			setRightSide(turn);
			setLeftSide(-turn);
		}else {
			driveOnPosition = true;
			setDrive(0);
		}
	}
	wait1Msec(500);
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
