#include "PID.c"
PID driveLeftPosition, driveRightPosition, anglePosition; // Control Loops for each function of the drive
bool driveOnPosition = true;
float driveAngle = 0.0; // Current Angle
int driveEncoderGoal = 0;
int driveAngleGoal = 0;
bool encoderGoalPriority = false, angleGoalPriority = false; // Tell which function of the drive is being given priority

// Zeros drive encoders
void resetEncoders() {
	SensorValue[leftEn] = 0;
	nMotorEncoder[leftFront] = 0;
	SensorValue[rightEn] = 0;
	nMotorEncoder[rightFront] = 0;
}

void waitForDrive(int timeout) {
	clearTimer(T4);
	while ( !driveOnPosition && time1[T4] < timeout){wait1Msec(150); }
	encoderGoalPriority = false;
	angleGoalPriority = false;
}

// No timeout
void waitForDrive() {
	while (!driveOnPosition){
		wait1Msec(150);
	}
	writeDebugStreamLine("IS DONE");
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

// inits in navigator
void initDrive() {
	PIDInit(driveLeftPosition, 3.0, 0.0, 10.0); // kP, kI, kD, simple PD controller will do for this drive
	PIDInit(driveRightPosition, 3.0, 0.00, 10.0);
	PIDInit(anglePosition, 4.1, 0, 2.5);
}

task driveNavigator() {
	initDrive();
	while(true) {
		if (!isEncoderOnTarget() && encoderGoalPriority) { // Fix translational error
			driveOnPosition = false;
			//driveRightPosition.print = true;
			// driveLeftPosition.print = true;
			// Run PD Controlelr to find error in translational movement and how to fix it
			float rightThrottle = PIDRun(driveRightPosition, (float)(driveEncoderGoal - getRightSideMM()));
			float leftThrottle = PIDRun(driveLeftPosition, (float)(driveEncoderGoal - getRightSideMM()));
			setRightSide(rightThrottle);
			setLeftSide(leftThrottle);
		}else if (!isAngleOnTarget() && angleGoalPriority) { // Fix Angular Error
			driveOnPosition = false;
			// PD controlelr to fix angle
			float turn = PIDRun(anglePosition, (float)(driveAngleGoal - driveAngle));
			setRightSide(turn);
			setLeftSide(-turn);
		}else { // Make sure others know that drive has met goal
			driveOnPosition = true;
			setDrive(0);
		}
	}
	wait1Msec(100);
}

// Add offset to the angle
void addToAngle(float angleAdd) {
	stopTask(driveNavigator);
	driveAngleGoal = driveAngle - angleAdd;
	angleGoalPriority = true;
	driveOnPosition = false;
	startTask(driveNavigator);
	wait1Msec(500);
}

// Add offset (mm) to the translational count
void moveForwardMM(float mm) {
	stopTask(driveNavigator);
	resetEncoders();
	driveEncoderGoal = mm;
	encoderGoalPriority = true;
	driveOnPosition = false;
	startTask(driveNavigator);
	wait1Msec(500);
}

// Slowly goe at a certain speed
void moveForwardMMSlow(float mm) {
	stopTask(driveNavigator);
	resetEncoders();
	while(getRightSideMM() < mm) {
		setDrive(40);
	}
	setDrive(0);
}

task driveTracker() {
	float lastRightRotations = getRightSideRotations();
	float lastLeftRotations = getLeftSideRotations();
	float degreesPerRotations = ROBOT_DEG_PER_ROTATION;
	while (true) {
		float rightSide = getRightSideRotations();
		float leftSide = getLeftSideRotations();
		// delta Angle = delta Right - delta Left * degPerRotations
		driveAngle += ( (rightSide - lastRightRotations) - (leftSide - lastLeftRotations) )	* degreesPerRotations;
		// Debugging Statements
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
