//
// Created by Abiel on 10/16/17.
//

#include "EctoTank.h"

/**
 * Constructor, initializer list requires rightMaster and leftMaster motor reference, also the name of the EctoSubsytem
 */
EctoTank::EctoTank() : rightMaster(*motors.getMotor("RightMaster")), leftMaster(*motors.getMotor("LeftMaster")),
                       DriveTrain("Tank Drive") {
	log->trace("Initializing EctoTank...");
	
	speedLedFeedback = new DigitalOutput(9);
	
	pathChooser = new SendableChooser<string>();
	updatePaths();
	updateSettings();
	
	
	setupPids();
	if (isSpeedShift()) {
		enableShift();
	} else {
		disableShift();
	}
}
/**
 * Updates the paths available found on the paths folder
 */
void EctoTank::updatePaths() {
	delete pathChooser;
	pathChooser = new SendableChooser<string>();
	pathChooser->AddDefault("Default", "default");
	for(const auto &pathName : parser.getPathsOnDir()){
		pathChooser->AddObject(pathName, pathName);
	}
	
	SmartDashboard::PutData("Select Path", pathChooser);
	
}

/**
 * Updates settings read by the config file and general settings, such as brake state, slave motors, etc.
 */
void EctoTank::updateSettings() {
	
	// BOOST_LOG_TRIVIAL(trace) << "EctoTank initializing...";
	string pathChoosed;
	
	pathChoosed = pathChooser->GetSelected();
	path.reset(new Path(*parser.getPath(pathChoosed)));
	
	
	log->trace("Updating Settings...");
	
	settings.setCurrentGroup("Tank");
	
	log->trace("Getting slave motors...");
	
	leftSlaves = {motors.getMotor("LeftSlave1"), motors.getMotor("LeftSlave2")};
	
	rightSlaves = {motors.getMotor("RightSlave1"), motors.getMotor("RightSlave2")};
	
	
	setupSlaves();
	
	minSpeedShiftEnabled = settings.getNumber("MinSpeedShift");
	
	maxSpeed_Speed  = settings.getNumber("MaxSpeed_Speed");
	maxSpeed_Torque = settings.getNumber("MaxSpeed_Torque");
	
	expoJoy = settings.getNumber("ExpoJoy");
	
	expoTrig = settings.getNumber("ExpoTrig");
	
	speedPid  = *EctoUtils::readPIDs("Speed");
	torquePid = *EctoUtils::readPIDs("Torque");
	
	settings.setCurrentGroup("EctoState");
	wheelDiameter = settings.getNumber("WheelDiameter");
	
	log->trace("Setting speed mode to {}", speedShift);
	
	if (speedShift) {
		setSpeedMode();
	} else {
		setTorqueMode();
	}
	
	log->trace("Setting brake mode to {}", brakeState);
	
	setBrakes(brakeState);
}

/**
 * Configures slave motors to follo masters
 */
void EctoTank::setupSlaves() {
	log->trace("Setting up slave motors...");
	
	for (const auto &motor : leftSlaves) {
		motors.setupSlave(*motor, leftMaster);
	}
	
	for (const auto &motor : rightSlaves) {
		motors.setupSlave(*motor, rightMaster);
	}
}

/**
 * Writes PIDs to motors
 */
void EctoTank::setupPids() {
	motors.configPID(leftMaster, speedPid, speedProfileSlot);
	motors.configPID(leftMaster, torquePid, torqueProfileSlot);
	
	motors.configPID(rightMaster, speedPid, speedProfileSlot);
	motors.configPID(rightMaster, torquePid, torqueProfileSlot);
	
	leftMaster.SetVoltageRampRate(voltageRampRate);
	rightMaster.SetVoltageRampRate(voltageRampRate);
	
	leftMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	rightMaster.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	
	leftMaster.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	rightMaster.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	
	leftMaster.Set(0);
	rightMaster.Set(0);
};

/**
 * Changes the current drivemode
 * @param mode
 */
void EctoTank::setDriveMode(TankDriveMode mode) {
	//BOOST_LOG_TRIVIAL(trace) << "Setting DriveMode for Tank to= " << mode;
	motors.getMotor("LeftMaster")->SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	motors.getMotor("RightMaster")->SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	
	currentMode = mode;
}

/**
 * Returns the current drive mode
 * */
TankDriveMode EctoTank::getDriveMode() {
	return currentMode;
}

/**
 * Changes profiles slots on left master and right master
 * @param profileSlot
 */
void EctoTank::selectProfileSlots(const unsigned int profileSlot) {
	leftMaster.SelectProfileSlot(profileSlot);
	rightMaster.SelectProfileSlot(profileSlot);
	
	currentProfile = profileSlot;
}

/**
 * Changes profile slots to speed mode
 */
void EctoTank::setSpeedMode() {
	selectProfileSlots(speedProfileSlot);
}

/**
 * Changes profile slots to torque mode
 */
void EctoTank::setTorqueMode() {
	selectProfileSlots(torqueProfileSlot);
}

/**
 * Returns if the current intent is to be on speed
 * @return
 */
bool EctoTank::isSpeedShift() const {
	return speedShift;
}

/**
 * Returns if the speed piston is actually engaged
 * @return
 */
bool EctoTank::isSpeedPistonEnabled() const {
	return speedPistonEnabled;
}

/**
 * Toggles between torque and speed intent.
 */
void EctoTank::toggleShift() {
	// BOOST_LOG_TRIVIAL(trace) << "Shift piston toggle on Tank";
	speedShift = !speedShift;
}

/**
 * Enables the intent to set the piston shift on speed, will only do so if speed is above certain treshold
 */
void EctoTank::enableShift() {
	speedShift = true;
	
	if (isInSpeedTimeout) {
		double timeSinceTimeout = std::chrono::duration_cast<std::chrono::duration<double>>(
				Time::now() - lastSpeedChange).count();
		if (timeSinceTimeout > speedChangeTimeoutSec) {
			isInSpeedTimeout = false;
		}
	} else {
		if ((abs(getRightMotorsSpeed()) + abs(getLeftMotorsSpeed())) / 2.0 > minSpeedShiftEnabled) {
			auto previousMode = rightMaster.GetTalonControlMode();
			
			rightMaster.SetTalonControlMode(CANTalon::TalonControlMode::kVoltageMode);
			leftMaster.SetTalonControlMode(CANTalon::TalonControlMode::kVoltageMode);
			
			setBrakes(false);
			setRightMotorsSpeed(0);
			setLeftMotorsSpeed(0);
			isInSpeedTimeout = true;
			lastSpeedChange  = Time::now();
			
			pcm.setPiston("Shift", speedShift);
			setSpeedMode();
			speedPistonEnabled = true;
			setBrakes(true);
			
			rightMaster.SetTalonControlMode(previousMode);
			leftMaster.SetTalonControlMode(previousMode);
			
		} else {
			pcm.setPiston("Shift", false);
			speedPistonEnabled = false;
			
			setTorqueMode();
		}
	}
	
}

/**
 * Disables speed shift intent and sets piston to torque
 */
void EctoTank::disableShift() {
	// BOOST_LOG_TRIVIAL(trace) << "Shift piston toggle on Tank";
	speedShift = false;
	pcm.setPiston("Shift", speedShift);
	setTorqueMode();
	
}

/**
 * Toggles inverted state.
 */
void EctoTank::toggleInverted() {
	inverted = !inverted;
	// BOOST_LOG_TRIVIAL(trace) << "Changing inverted on tank to= " << inverted;
}

/**
 * Toggles QuickTurn
 */
void EctoTank::toggleQuickTurn() {
	quickTurn = !quickTurn;
	// BOOST_LOG_TRIVIAL(trace) << "Changing quick turn status on tank to=" << inverted;
}

/**
 * Writes to motors.
 * @param in
 */
void EctoTank::writeMotors(shared_ptr<MotorValues> in) {
	auto temp = in->readValueTank();
	
	double left  = temp->left * getMaxSpeed();
	double right = temp->right * getMaxSpeed();
	
	///Uncomment this to enable motors
	setLeftMotorsSpeed(left);
	setRightMotorsSpeed(right);
}

/**
 * Calculate motor values for a tank drive.
 * @param inverted
 * @return
 */
shared_ptr<MotorValues> EctoTank::tankDrive(bool inverted) {
	double invert = inverted ? -1 : 1;
	
	double leftY     = stick.getJoystickExpo("leftY", expoJoy, 0);
	double rightY    = stick.getJoystickExpo("rightY", expoJoy, 0);
	//cout << leftY << " " << rightY << endl;
	double leftTrig  = stick.getJoystickExpo("leftTrig", expoTrig, 0);
	double rightTrig = stick.getJoystickExpo("rightTrig", expoTrig, 0);
	
	double leftPush  = stick.getButtonState("leftPush");
	double rightPush = stick.getButtonState("rightPush");
	
	tankValues output{};
	
	if (leftPush) {
		output.left  = -0.75;
		output.right = 0.75;
	} else if (rightPush) {
		output.left  = 0.75;
		output.right = -0.75;
		
	} else if (rightTrig != 0) {
		output.left  = invert * rightTrig;
		output.right = invert * rightTrig;
		
	} else if (leftTrig != 0) {
		output.left  = invert * -leftTrig;
		output.right = invert * -leftTrig;
		
	} else if (leftY != 0 || rightY != 0) {
		if (!inverted) {
			output.left  = -leftY;
			output.right = -rightY;
		} else {
			output.left  = rightY;
			output.right = leftY;
		}
	} else {
		output.left  = 0;
		output.right = 0;
	}
	return make_shared<MotorValues>(output);
}

/**
 * Implements curve drive using single joystick.
 * @param isQuickTurn
 * @return
 */
shared_ptr<MotorValues> EctoTank::curveDriveSingleJoy(bool isQuickTurn) {
	double x = stick.getJoystickExpo("leftX", expoJoy, 0);
	double y = stick.getJoystickExpo("leftY", expoJoy, 0);
	
	double magnitude = sqrt(pow(x, 2.0) + pow(y, 2.0));
	double angle     = atan2(x, y);
	
	// cout << "Magnitude: " << magnitude << endl;
	// cout << "Angle: " << angle << endl;
	
	return curveDrive(magnitude, angle, isQuickTurn);
}

/**
 * Implements curve drive using trigger and left joystick for steering.
 * @param isQuickTurn
 * @return
 */
shared_ptr<MotorValues> EctoTank::curveDriveWheel(bool isQuickTurn, bool invert) {
	double dir      = invert ? 1 : -1;
	double y        = dir * stick.getJoystickExpo("leftY", expoJoy, 0);
	double rotation = stick.getJoystickExpo("leftX", expoJoy, 0);
	
	//cout << "no lo que quiero" << endl;
	//cout << "Y: " << y << endl;
	//cout << "Rotation : " << rotation << endl;
	
	return curveDrive(y, rotation, isQuickTurn);
}

/**
 * Implements curve drive.
 * @param y
 * @param rotation
 * @param isQuickTurn
 * @return
 */
shared_ptr<MotorValues> EctoTank::curveDrive(double y, double rotation, bool isQuickTurn) {
	double angularPower;
	bool   overPower;
	
	tankValues temp{};
	
	if (isQuickTurn) {
		if (abs(y) < 0.2) {
			quickStopAccumulator = (1 - alpha) * quickStopAccumulator + alpha * EctoUtils::limit(rotation) * 2;
		}
		
		overPower    = true;
		angularPower = rotation;
	} else {
		overPower    = false;
		angularPower = abs(y) * rotation - quickStopAccumulator;
		
		if (quickStopAccumulator > 1) {
			quickStopAccumulator -= 1;
		} else if (quickStopAccumulator < -1) {
			quickStopAccumulator += 1;
		} else {
			quickStopAccumulator = 0.0;
		}
	}
	
	double leftMotorOutput  = y + angularPower;
	double rightMotorOutput = y - angularPower;
	
	// If rotation is overpowered, reduce both outputs to within acceptable range
	if (overPower) {
		if (leftMotorOutput > 1.0) {
			rightMotorOutput -= leftMotorOutput - 1.0;
			leftMotorOutput = 1.0;
		} else if (rightMotorOutput > 1.0) {
			leftMotorOutput -= rightMotorOutput - 1.0;
			rightMotorOutput = 1.0;
		} else if (leftMotorOutput < -1.0) {
			rightMotorOutput -= leftMotorOutput + 1.0;
			leftMotorOutput = -1.0;
		} else if (rightMotorOutput < -1.0) {
			leftMotorOutput -= rightMotorOutput + 1.0;
			rightMotorOutput = -1.0;
		}
	}
	
	temp.left  = leftMotorOutput;
	temp.right = rightMotorOutput;
	
	return make_shared<MotorValues>(temp);
}

/**
 * Sets the speed of the left motors on meters per second
 * @param speed
 */
void EctoTank::setLeftMotorsSpeed(double speed) {
	double outputSpeed = speed;
	if (abs(outputSpeed) <= getMaxSpeed()) {
		double output = EctoUtils::MSToRPM(outputSpeed, wheelDiameter);
		leftMaster.Set(output);
	}
}

/**
 * Sets the speed of the right motors on meters per second
 * @param speed
 */
void EctoTank::setRightMotorsSpeed(double speed) {
	double outputSpeed = speed;
	if (abs(outputSpeed) <= getMaxSpeed()) {
		double output = EctoUtils::MSToRPM(outputSpeed, wheelDiameter);
		rightMaster.Set(output);
	}
}

/**
 * Returns the speed of the left motors on meters per second
 * @return
 */
double EctoTank::getLeftMotorsSpeed() const {
	double rpm   = leftMaster.getSpeedRPM();
	double speed = EctoUtils::RPMToMS(rpm, wheelDiameter);
	
	return speed;
}

/**
 * Returns the speed of the right motors on meters per second
 * @return
 */
double EctoTank::getRightMotorsSpeed() const {
	double rpm   = rightMaster.getSpeedRPM();
	double speed = EctoUtils::RPMToMS(rpm, wheelDiameter);
	
	return speed;
}

/**
 * Sets the brakes state
 * @param state
 */
void EctoTank::setBrakes(const bool state) {
	leftMaster.setBrake(state);
	rightMaster.setBrake(state);
}

/**
 * Returns the max speed the robot can be expected to reach on the current speed mode
 * @return
 */
double EctoTank::getMaxSpeed() const {
	double maxSpeed;
	if (isSpeedPistonEnabled()) {
		maxSpeed = maxSpeed_Speed;
	} else {
		maxSpeed = maxSpeed_Torque;
	}
	return maxSpeed;
}

/**
 * Returns the min speed the robot can be expected to move at on the current speed mode
 * @return
 */
double EctoTank::getMinSpeed() const {
	return getMaxSpeed() * .15;
}

/**
 * Returns the yaw of the robot
 * @return
 */
double EctoTank::getYaw() {
	return navx.GetYaw();
}

/**
 * Initializes disabled
 */
void EctoTank::initDisabled() {

}

/**
 * Update disabled
 */
void EctoTank::updateDisabled() {
	updatePaths();
	
	///Read that this is useful when the robot is on for long periods of time before a match, so that it doesnt drift.
	navx.Reset();
	navx.ZeroYaw();
}

/**
 * Initializes teleop
 */
void EctoTank::initTeleop() {
	setDriveMode(TankDriveMode::TankDrive);
	updateSettings();
	
}

/**
 * Update tank teleop function.
 */
void EctoTank::updateTeleop() {
	bool xBttn = stick.getButtonState("X");
	bool aBttn = stick.getButtonState("A");
	bool yBttn = stick.getButtonState("Y");
	
	if (xBttn != lastX && xBttn) {
		toggleShift();
	}
	
	if (speedShift)
		enableShift();
	else
		disableShift();
	
	if (aBttn != lastA && aBttn) {
		toggleInverted();
	}
	
	if (yBttn != lastY && yBttn) {
		toggleQuickTurn();
	}
	
	shared_ptr<MotorValues> value;
	
	switch (currentMode) {
		case TankDriveMode::CurveDrive:
			//Temporary implementation
			value = curveDriveWheel(quickTurn, inverted);
			writeMotors(value);
			break;
		case TankDriveMode::TankDrive:
			value = tankDrive(inverted);
			writeMotors(value);
			break;
	}
	lastX      = xBttn;
	lastA      = aBttn;
	lastY      = yBttn;
	//cout << "Distance: " << state.getDistanceTraveled() << endl;
	//state.printLastState();
	
}

/**
 * Initializes autonomous
 */
void EctoTank::initAuto() {
	updateSettings();
	runner.reset(new PurePursuitRunner(*path, .5));
}

/**
 * Updates autonomous function
 */
void EctoTank::updateAuto() {
	///PATHS WORK
	runner->runPath(*this, .5);
}

/**
 * Sets up initial telemetry for Tank
 */
void EctoTank::drivetrainInitialTelemetry() const {
}

/**
 * Updates telemetry for Tank
 */
void EctoTank::drivetrainTelemetry() const {
	getMaxSpeed();
	speedLedFeedback->Set(isSpeedShift());
	SmartDashboard::PutBoolean("Inverted", inverted);
	SmartDashboard::PutBoolean("SpeedShift", isSpeedShift());
	SmartDashboard::PutNumber("SpeedRobotRight", getRightMotorsSpeed());
	SmartDashboard::PutNumber("SpeedRobotLeft", getLeftMotorsSpeed());
}




