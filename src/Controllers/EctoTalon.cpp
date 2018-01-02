//
// Created by alberto on 13/12/17.
//
#include "EctoTalon.h"

EctoTalon::EctoTalon(int id) : CANTalon(id) {

}

double EctoTalon::getSpeedRPM() const {
	return this->GetSpeed();
}

double EctoTalon::getSpeedMS(double wheelDiameter) const {
	return EctoUtils::RPMToMS(getSpeedRPM(), wheelDiameter);
}

/**
 * If brake state is true, brakes are enabled, if false coast mode is enabled
 * @param brakeState
 */
void EctoTalon::setBrake(bool brakeState) {
	if (brakeState)
		this->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
	else
		this->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Coast);
}