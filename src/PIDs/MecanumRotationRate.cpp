#include "MecanumRotationRate.h"

MecanumRotationRate::MecanumRotationRate() = default;

void MecanumRotationRate::updateSpeeds() {
	this->x = mecanum.xOut;
	this->y = mecanum.yOut;
}

void MecanumRotationRate::PIDWrite(double output) {
	updateSpeeds();
	
	auto values = kinematics.inverseKinematics(x, y, output);
	
	mecanum.writeMotors(values);
}
