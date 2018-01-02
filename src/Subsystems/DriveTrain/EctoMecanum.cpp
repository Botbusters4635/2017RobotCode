#include "EctoMecanum.h"
#include "PIDs/MecanumRotationRate.h"

EctoMecanum::EctoMecanum() :
		frontLeft(*motors.getMotor("Front Left")),
		frontRight(*motors.getMotor("Front Right")),
		backLeft(*motors.getMotor("Back Left")),
		backRight(*motors.getMotor("Back Right")), DriveTrain("Mecanum DriveTrain") {
	
	settings.setCurrentGroup("Mecanum");
	
	strafeExpo = settings.getNumber("StrafeExpo");
	forwardsExpo = settings.getNumber("ForwardExpo");
	rotExpo = settings.getNumber("RotExpo");
	
	pRot = settings.getNumber("P_Rot");
	iRot = settings.getNumber("I_Rot");
	dRot = settings.getNumber("D_Rot");
	tolRot = settings.getNumber("Tol_Rot");
	
	rotRate = settings.getNumber("RotationRate");
	
	maximumSpeed = settings.getNumber("MaxSpeed");
	
	setupPIDs();
	configPIDs(0);
	
	sourcePID = new NavXYawPID();
	outputPID = new MecanumRotationRate();
	
	controller = new PIDController(pRot, iRot, dRot, sourcePID, outputPID);
	
	controller->SetInputRange(-1, 1);
	controller->SetOutputRange(-1 * rotRate, rotRate);
	
	controller->SetAbsoluteTolerance(abs(rotRate));
	
	sourcePID->resetYaw();
	
	controller->Enable();
}

//TODO Implement telemetry for mecanum
void EctoMecanum::drivetrainInitialTelemetry() const {
	;
}

void EctoMecanum::drivetrainTelemetry() const {
	;
}

/**
 * Sets up PIDs for individual motors.
 */
void EctoMecanum::setupPIDs() {
	settings.setCurrentGroup("Mecanum");
	
	frontLeft.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	frontRight.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	backLeft.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	backRight.SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	
	frontLeft.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	frontRight.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	backLeft.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
	backRight.SetTalonControlMode(CANTalon::TalonControlMode::kSpeedMode);
}

/**
 * Writes PID configs for individual motors, from settings.
 * @param profileSlot Profile Slot
 */
void EctoMecanum::configPIDs(const unsigned int profileSlot) {
	settings.setCurrentGroup("Mecanum");
	
	vector<double> p = settings.getArrayNumbers("MotorP");
	vector<double> i = settings.getArrayNumbers("MotorI");
	vector<double> d = settings.getArrayNumbers("MotorD");
	vector<double> f = settings.getArrayNumbers("MotorF");
	
	//Writes PIDs to talons
	motors.configPID(frontLeft, p[0], i[0], d[0], f[0], profileSlot);
	motors.configPID(frontRight, p[1], i[1], d[1], f[1], profileSlot);
	motors.configPID(backLeft, p[2], i[2], d[2], f[2], profileSlot);
	motors.configPID(backRight, p[3], i[3], d[3], f[3], profileSlot);
}

/**
 * Writes to motors.
 * @param in Motor Values.
 */
void EctoMecanum::writeMotors(shared_ptr<MotorValues> in) {
	auto driveTrain = in->getDriveTrain();
	
	if (driveTrain == DriveTrainSetting::Mecanum) {
		auto values = in->readValueMecanum();
		
		frontLeft.Set(values->frontLeft);
		frontRight.Set(values->frontRight);
		
		backLeft.Set(values->backLeft);
		backRight.Set(values->backRight);
	} else if (driveTrain == DriveTrainSetting::Tank) {
		auto values = in->readValueTank();
		
		frontLeft.Set(values->left);
		backLeft.Set(values->left);
		
		frontRight.Set(values->right);
		backRight.Set(values->right);
	} else {
		throw logic_error("Value not mecanum nor tank!");
	}
}

/**
 * Updates inputs and writes them to PID Loop
 */
void EctoMecanum::update() {
	double x = stick.getJoystickExpo("leftX", forwardsExpo, 0);
	double y = stick.getJoystickExpo("leftY", strafeExpo, 0);
	double rotationRate = stick.getJoystickExpo("rightX", rotExpo, 0);
	
	auto value = kinematics.inverseKinematicsField(x, y, gyro.GetYaw());
	
	xOut = EctoUtils::map(value->getX(), -1.0, 1.0, -1.0 * maximumSpeed, maximumSpeed);
	yOut = EctoUtils::map(value->getY(), -1.0, 1.0, -1.0 * maximumSpeed, maximumSpeed);
	
	controller->SetSetpoint(rotationRate);
}

void EctoMecanum::initTeleop() {

}

void EctoMecanum::updateTeleop() {
	update();
}

void EctoMecanum::initAuto() {

}

void EctoMecanum::updateAuto() {
}

void EctoMecanum::initDisabled() {
}

void EctoMecanum::updateDisabled() {
}

//TODO IMPLEMENT ALL OF THIS
void EctoMecanum::setLeftMotorsSpeed(double speed) {
	cout << "NOT YET IMPLEMENTED ON ECTOTMECANUM" << endl;
	assert(false);
}

void EctoMecanum::setRightMotorsSpeed(double speed) {
	cout << "NOT YET IMPLEMENTED ON ECTOTMECANUM" << endl;
	assert(false);
}

double EctoMecanum::getRightMotorsSpeed() const {
	cout << "NOT YET IMPLEMENTED ON ECTOTMECANUM" << endl;
	assert(false);
}

double EctoMecanum::getLeftMotorsSpeed() const {
	cout << "NOT YET IMPLEMENTED ON ECTOTMECANUM" << endl;
	assert(false);
}

void EctoMecanum::setBrakes(bool state) {
	cout << "NOT YET IMPLEMENTED ON ECTOTMECANUM" << endl;
	assert(false);
}

double EctoMecanum::getMaxSpeed() const {
	cout << "NOT YET IMPLEMENTED ON ECTOTMECANUM" << endl;
	assert(false);
	return -1;
}

double EctoMecanum::getYaw() {
	return gyro.GetYaw();
}

double EctoMecanum::getMinSpeed() const {
	return getMaxSpeed() * .15;
}

