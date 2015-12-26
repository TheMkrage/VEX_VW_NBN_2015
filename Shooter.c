
void setShooter(float power) {
	motor[leftIntake] = power;
	motor[rightIntake] = power;
}

void neutral() {
	motor[elevator] = 0;
	setShooter(0);
}

void intake() {
	motor[elevator] = -127;
	setShooter(-127);
}

void shoot(int initial) {
	waitForArm(10000);
	setShooter(initial);
	wait1Msec(400);
	setShooter(127);
	motor[elevator] = 127;
	wait1Msec(1000);
	setShooter(0);
	motor[elevator] = 0;
}
	//Back and forth SP style;
	/*
	moveArmTo(350);
	addToAngle(-98);
	waitForDrive();
	moveForwardMM(1200);
	waitForDrive();
	wait1Msec(1000);
	setShooter(80);
	wait1Msec(500);
	setShooter(127);
	motor[elevator] = 127;
	wait1Msec(1000);
	setShooter(0);
	motor[elevator] = 0;
	while(true) {
		moveForwardMM(-2250);
		waitForDrive();
		wait1Msec(500);
		moveForwardMM(2200);
		waitForDrive();
		setShooter(80);
		wait1Msec(500);
		setShooter(127);
		motor[elevator] = 127;
		wait1Msec(1000);
		motor[elevator] = 0;
		setShooter(0);
	}*/

	// 159 Strat
		/*setShooter(127);
	motor[elevator] = 127;
	wait1Msec(500);
	addToAngle(-26);
	waitForDrive();
	setShooter(-127);
	motor[elevator] = -127;
	moveForwardMM(1200);
	waitForDrive();
	setShooter(0);
	motor[elevator] = 0;
	moveArmTo(350);
	addToAngle(-112);
	waitForDrive();
	moveForwardMM(1200);
	waitForDrive();
	shoot(105);
	moveArmTo(0);
	moveForwardMM(100);
	waitForDrive();

	// get stack 2
	motor[elevator] = -127;
	setShooter(-127);
	addToAngle(-154);
	waitForDrive();
	moveForwardMM(500);
	waitForDrive();
	moveArmTo(600);
	motor[elevator] = 0;
	setShooter(0);
	moveForwardMM(-500);
	waitForDrive();

	addToAngle(-46);
	waitForDrive();
	shoot(90); // shoot stack 2

	moveArmTo(0);
	addToAngle(80);
	waitForDrive();
	setShooter(-127);
	motor[elevator] = -127;
	moveForwardMM(410);
	waitForDrive();
	wait1Msec(500);
	moveArmTo(350);
	addToAngle(120);
	waitForDrive();
	shoot(90);

	// go to get stack 4
	moveArmTo(0);
	addToAngle(85);
	waitForDrive();
	intake();
	moveForwardMM(800);
	waitForDrive();
	neutral();
	moveArmTo(600);
	addToAngle(80);
	waitForDrive();
	moveForwardMM(-1000);
	waitForDrive();
	shoot(90);

	// go to get stack 5
	moveArmTo(0);
	moveForwardMM(350);
	waitForDrive();
	addToAngle(-83);
	waitForDrive();
	intake();
	moveForwardMM(940);
	waitForDrive();
	neutral();
	moveArmTo(350);
	moveForwardMM(300);
	waitForDrive();
	shoot(90);

	// go to get stack 6
	moveArmTo(0);
	moveForwardMM(-300);
	waitForDrive();
	intake();
	addToAngle(-85);
	waitForDrive(2500);
	moveForwardMM(100);
	waitForDrive();
	wait1Msec(500);
	moveForwardMM(-100);
	waitForDrive();
	addToAngle(-100);
	waitForDrive();
	moveForwardMM(200);
	waitForDrive();
	neutral();
	moveArmTo(600);
	moveForwardMM(-1800);
	waitForDrive();
	shoot(90);

	// next stack
	moveArmTo(0);
	intake();
	waitForArm(5000);
	moveForwardMM(880);
	waitForDrive();
	addToAngle(-20);
	waitForDrive();
	moveArmTo(600);
	neutral();
	moveForwardMM(-1000);
	waitForDrive();
	shoot(90);

	// next stack
	moveArmTo(0);
	moveForwardMM(400);
	waitForDrive();
	intake();
	addToAngle(-90);
	waitForDrive();
	moveForwardMM(900);
	waitForDrive();
	neutral();
	moveArmTo(600);
	addToAngle(45);
	waitForDrive();
	moveForwardMM(-300);
	waitForDrive();
	shoot(100);

	// last stack
	moveArmTo(0);
	intake();
	wait1Msec(750);
	addToAngle(-85);
	waitForDrive(2000);
	wait1Msec(500);
	moveForwardMM(400);
	waitForDrive();
	neutral();
	moveForwardMM(-400);
	waitForDrive();
	moveArmTo(350);
	addToAngle(-140);
	waitForDrive();
	setShooter(127);
	motor[elevator] = 127;*/

	// OLD PATH
	/*
	addToAngle(-46);
	waitForDrive(100000);
	setDrive(0);
	wait1Msec(250);
	moveArmTo(360);
	moveForwardMM(3000);
	waitForDrive(60000);

	setShooter(100);
	wait1Msec(500);
	setShooter(127);
	motor[elevator] = 127;
	wait1Msec(1000);
	moveArmTo(0);
	wait1Msec(1000);
	setShooter(-127);
	motor[elevator] = -127;
	addToAngle(-129);
	waitForDrive(3000);
	// GO TO MID STACK
	moveForwardMM(700);
	waitForDrive(20000);
	wait1Msec(250);
	moveArmTo(360);
	addToAngle(-27);
	waitForDrive(1000);
	moveForwardMM(450);
	setShooter(0);
	motor[elevator] = 0;
	waitForDrive(2000);
	setShooter(120);
	wait1Msec(500);
	setShooter(127);
	motor[elevator] = 127;
	wait1Msec(1250);
	moveArmTo(0);
	setShooter(-127);
	motor[elevator] = -127;
	addToAngle(-106);
	waitForDrive(3000);
	moveForwardMM(750);
	waitForDrive(3000);
	moveArmTo(360);
	motor[elevator] = 0;
	setShooter(0);
	addToAngle(122);
	waitForDrive(5000);
	moveForwardMM(300);
	waitForDrive(1500);
	setShooter(80);
	wait1Msec(500);
	setShooter(127);
	motor[elevator] = 127;
	wait1Msec(1250);
	while(true) {

		motor[elevator] = 0;
		setShooter(0);
		moveForwardMM(3500);
		waitForDrive(10000);
		wait1Msec(2000);
		moveForwardMM(-3500);
		waitForDrive(5000);
		stopTask(driveNavigator);
		setDrive(-127);
		wait1Msec(2000);
		setDrive(0);
		startTask(driveNavigator);
		setShooter(60);
		wait1Msec(500);
		setShooter(127);
		motor[elevator] = 127;
		wait1Msec(1000);
	}*/
