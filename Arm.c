double armGoal = 0;

bool isArmOnTarget() {
	return isInRangeOf(nMotorEncoder[arm], armGoal, 9);
}

void moveArmTo(double counts) {
	armGoal = counts;
}

task armNavigator {
	while(true) {
			if (!isArmOnTarget()) {
				if (nMotorEncoder[arm] < armGoal) {
					motor[arm] = 40;
				}else if (nMotorEncoder[arm] > armGoal ) {
					motor[arm] = -40;
				}
			}else {
				motor[arm] = 0;
			}
	}
}
