double armGoal = 0;

bool isArmOnTarget() {
	return isInRangeOf(nMotorEncoder[arm], armGoal, 9);
}

void waitForArm(int timeout) {
	clearTimer(T3);
	while (!isArmOnTarget() && time1[T3] < timeout) {
		wait1Msec(250);
	}
}

void moveArmTo(double counts) {
	armGoal = counts;
}

task armNavigator {
	while(true) {
			if (!isArmOnTarget()) {
				if (nMotorEncoder[arm] < armGoal) {
					motor[arm] = 60;
				}else if (nMotorEncoder[arm] > armGoal ) {
					motor[arm] = -60;
				}
			}else {
				motor[arm] = 0;
			}
	}
}
