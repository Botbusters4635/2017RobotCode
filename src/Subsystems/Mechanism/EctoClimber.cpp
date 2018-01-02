#include "EctoClimber.h"

EctoClimber::EctoClimber() : climbMotor(*motors.getMotor("ClimbMotor")), EctoSubsystem("Climber") {
	climbMotor.SetTalonControlMode(EctoTalon::TalonControlMode::kVoltageMode);
	climbMotor.Set(offSpeed);
}

/**
 * Returns the current motor state.
 * @return
 */
double EctoClimber::getClimberState() const {
	double motorState = climbMotor.Get();
	return motorState;
}


void EctoClimber::initTeleop() {
}

void EctoClimber::updateTeleop() {
	double output = stick.getButtonState("Y") ? onSpeed : offSpeed;
	climbMotor.Set(output);
}

void EctoClimber::initAuto() {

}

void EctoClimber::updateAuto() {

}

void EctoClimber::initDisabled() {

}

void EctoClimber::updateDisabled() {

}

void EctoClimber::sendInitialTelemetry() const {
	SmartDashboard::PutNumber("Climber State", getClimberState());
}

void EctoClimber::sendTelemetry() const {
	SmartDashboard::PutNumber("Climber State", getClimberState());
	
}



