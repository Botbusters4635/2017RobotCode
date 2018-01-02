#include "EctoKinematicsMecanum.h"

EctoKinematicsMecanum::EctoKinematicsMecanum() {
	settings.setCurrentGroup("Mecanum");
	
	lX = settings.getNumber("lX");
	lY = settings.getNumber("lY");
	
	//Defines lSum for optimization
	lSum = lX + lY;
}

/**
 * Inverse kinematics for mecanum.
 * @param  vX       X Velocity
 * @param  vY       Y Velocity
 * @param  rotation Rotation speed
 * @return
 */
shared_ptr<MotorValues> EctoKinematicsMecanum::inverseKinematics(const double vX,
                                                                 const double vY,
                                                                 const double rotation) {
	mecanumValues calc{};
	
	calc.frontLeft = (vX - vY - (1) * rotation);
	calc.frontRight = (vX + vY + (1) * rotation);
	calc.backLeft = (vX + vY - (1) * rotation);
	calc.backRight = (vX - vY + (1) * rotation);
	
	auto output = make_shared<MotorValues>(calc);
	
	double maxSpeed = output->calculateMax();
	maxSpeed = abs(maxSpeed) < 1 ? 1 : maxSpeed;
	
	output->limitValues(maxSpeed * -1, maxSpeed, -1, 1);
	
	return output;
}

/**
 * Inverse kinematics for mecanum, returns in units.
 * @param  vX       X Velocity
 * @param  vY       Y Velocity
 * @param  rotation Rotation Speed
 * @param  radius   Wheel Radius
 * @return
 */
shared_ptr<MotorValues> EctoKinematicsMecanum::inverseKinematics(const double vX,
                                                                 const double vY,
                                                                 const double rotation,
                                                                 const double radius) {
	double radiusCalc = 1 / radius;
	
	mecanumValues calc{};
	
	calc.frontLeft = radiusCalc * (vX - vY - (lSum) * rotation);
	calc.frontRight = radiusCalc * (vX + vY + (lSum) * rotation);
	calc.backLeft = radiusCalc * (vX + vY - (lSum) * rotation);
	calc.backRight = radiusCalc * (vX - vY + (lSum) * rotation);
	
	auto output = make_shared<MotorValues>(calc);
	return output;
}

shared_ptr<MotorValues> EctoKinematicsMecanum::inverseKinematics(const Twist2d &in) {
	return inverseKinematics(in.getX(), in.getY(), in.getTheta());
}

shared_ptr<MotorValues> EctoKinematicsMecanum::inverseKinematics(const Twist2d &in, const double radius) {
	return inverseKinematics(in.getX(), in.getY(), in.getTheta(), radius);
}

/**
 * Applies field oriented transformations to a set of velocities
 * @param  vX       X Velocity
 * @param  vY       Y Velocity
 * @param  gyro     Gyro Yaw
 * @return
 */
shared_ptr<Twist2d> EctoKinematicsMecanum::inverseKinematicsField(const double vX, const double vY, const double gyro) {
	double cosA = cos(EctoUtils::degToRad(gyro));
	double sinA = sin(EctoUtils::degToRad(gyro));
	
	double xOut = vX * cosA - vY * sinA;
	double yOut = vX * sinA + vY * cosA;
	
	auto output = make_shared<Twist2d>(xOut, yOut, 0);
	
	return output;
}

shared_ptr<Twist2d> EctoKinematicsMecanum::inverseKinematicsField(const Twist2d &in) {
	return inverseKinematicsField(in.getX(), in.getY(), in.getTheta());
}

//TODO Check kinematics formulas
/**
 * Calculates velocities using encoder inputs in rotations per second
 * @param  in Encoder Values in rps
 * @return
 */
const shared_ptr<Twist2d> EctoKinematicsMecanum::kinematics(shared_ptr<MotorValues> in) {
	auto temp = in->readValueMecanum();
	
	//TODO Confirm if formulas are correct
	double vX = (temp->frontLeft + temp->frontRight + temp->backLeft + temp->backRight) * .25;
	
	double vY = (-1 * temp->frontLeft + temp->frontRight + temp->backLeft - temp->backRight) * .25;
	
	double angularVel = (-1 * temp->frontLeft + temp->frontRight - temp->backLeft + temp->backRight) * .25;
	
	return make_shared<Twist2d>(vX, vY, angularVel);
}

/**
 * Calculates velocities using encoder inputs in m/s
 * @param  in     Encoder Values in m/s
 * @param  radius Wheel radius
 * @return
 */
const shared_ptr<Twist2d> EctoKinematicsMecanum::kinematics(shared_ptr<MotorValues> in, const double radius) {
	auto temp = in->readValueMecanum();
	
	double calc = radius / 4;
	
	//TODO Confirm if formulas are correct
	double vX = (temp->frontLeft + temp->frontRight + temp->backLeft + temp->backRight) * calc;
	
	double vY = (-1 * temp->frontLeft + temp->frontRight + temp->backLeft - temp->backRight) * calc;
	
	double angularVel =
			(-1 * temp->frontLeft + temp->frontRight - temp->backLeft + temp->backRight) * (radius / (4 * (lX + lY)));
	
	return make_shared<Twist2d>(vX, vY, angularVel);
}
