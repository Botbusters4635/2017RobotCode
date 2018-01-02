#include "EctoKinematicsTank.h"

EctoKinematicsTank::EctoKinematicsTank() {
	settings.setCurrentGroup("Tank");
	
	wheelWidth = settings.getNumber("WheelWidth");
	//TODO Find out what this is
	tankScrubFactor = settings.getNumber("TankScrubFactor");
}

//TODO Check the following functions

/**
 * Calculate speed to move the motors at given velocity.
 * @param  vX       X Velocity
 * @param  vY       Y Velocity (Not used)
 * @param  rotation Rotation Value
 * @return
 */
shared_ptr<MotorValues> EctoKinematicsTank::inverseKinematics(const double vX, const double vY, const double rotation) {
	if (vY != 0)
		throw logic_error("Attempted to calculate inverse kinematics for tank with a Y value! This is not possible.");
	
	tankValues temp{};
	
	//If values are 0
	if (abs(rotation) < std::numeric_limits<double>::epsilon()) {
		temp.left = vX;
		temp.right = vX;
	}
	
	//TODO Implement scrub values if needed
	double delta_v = rotation / 2;
	temp.left = vX - delta_v;
	temp.right = vX + delta_v;
	
	auto output = make_shared<MotorValues>(temp);
	return output;
}

shared_ptr<MotorValues> EctoKinematicsTank::inverseKinematics(const double vX,
                                                              const double vY,
                                                              const double rotation,
                                                              const double radius) {
	auto temp = inverseKinematics(vX, vY, rotation);
	auto calc = temp->readValueTank();
	
	calc->left = calc->left * M_PI * 2 * radius;
	calc->right = calc->left * M_PI * 2 * radius;
	
	temp->writeValue(*calc);
	return temp;
}

shared_ptr<MotorValues> EctoKinematicsTank::inverseKinematics(const Twist2d &in) {
	return inverseKinematics(in.getX(), in.getY(), in.getTheta());
}

shared_ptr<MotorValues> EctoKinematicsTank::inverseKinematics(const Twist2d &in, const double radius) {
	return inverseKinematics(in.getX(), in.getY(), in.getTheta(), radius);
}

//TODO Check if formulas are correct
const shared_ptr<Twist2d> EctoKinematicsTank::kinematics(shared_ptr<MotorValues> in) {
	double gyro = in->getRotation();
	auto temp = in->readValueTank();
	
	double dx = (temp->left + temp->right) / 2.0;
	
	auto output = make_shared<Twist2d>(dx, 0, gyro);
	
	return output;
}

const shared_ptr<Twist2d> EctoKinematicsTank::kinematics(shared_ptr<MotorValues> in, const double radius) {
	auto calc = kinematics(in);
	
	calc->setX(calc->getX() * radius);
	
	return calc;
}
