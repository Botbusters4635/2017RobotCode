//
// Created by Alberto Jahuey on 11/30/2017.
//
#include "DriveTrain.h"

DriveTrain::DriveTrain(const string &name) : EctoSubsystem(name) {
	settings.setCurrentGroup("Telemetry");
	
	pdpId = settings.getNumber<uint8_t>("PdpId");
	
	pdp.reset(new PDP(pdpId));
}

void DriveTrain::driveFollowingAngle(double speed, double angle) {
	if (speed > getMaxSpeed()) {
		speed = getMaxSpeed();
	}
	if (speed < getMinSpeed()) {
		speed = getMinSpeed();
	}
	
	double angleError = EctoUtils::shortestAngleTurn(getYaw(), angle);
	double outputMultiplier = EctoUtils::map(abs(angleError), 0.0, maxErrorAngle, 0.0, 1.0);
	
	if (outputMultiplier > 1.0) {
		outputMultiplier = 1.0;
	} else if (outputMultiplier < 0.0) {
		outputMultiplier = 0.0;
	}
	if (abs(angleError) > angleErrorThreshold) {
		if (angleError > 0.0) {
			setLeftMotorsSpeed(speed);
			setRightMotorsSpeed(speed - (speed * outputMultiplier));
		} else if (angleError < 0.0) {
			setLeftMotorsSpeed(speed - (speed * outputMultiplier));
			setRightMotorsSpeed(speed);
		}
	} else {
		setLeftMotorsSpeed(speed);
		setRightMotorsSpeed(speed);
	}
}

bool DriveTrain::isAtAngle(double angle) {
	return (getYaw() > angle - angleErrorThreshold && getYaw() < angle + angleErrorThreshold);
}

void DriveTrain::turnToAngle(double angle) {
	double angleError = EctoUtils::shortestAngleTurn(getYaw(), angle);
	double outputMultiplier = EctoUtils::map(abs(angleError), 0.0, maxErrorAngle, 0.0, 1.0);
	
	if (outputMultiplier > 1.0) {
		outputMultiplier = 1.0;
	} else if (outputMultiplier < 0.0) {
		outputMultiplier = 0.0;
	}
	
	if (abs(angleError) > angleErrorThreshold) {
		if (angleError > 0.0) {
			setLeftMotorsSpeed(getMaxSpeed() * outputMultiplier);
			setRightMotorsSpeed(-getMaxSpeed() * outputMultiplier);
		} else if (angleError < 0.0) {
			setLeftMotorsSpeed(-getMaxSpeed() * outputMultiplier);
			setRightMotorsSpeed(getMaxSpeed() * outputMultiplier);
		}
	}
}

void DriveTrain::sendInitialTelemetry() const {
	string allianceString;
	DriverStation::Alliance alliance = DriverStation::GetInstance().GetAlliance();
	
	if (alliance == DriverStation::Alliance::kRed)
		SmartDashboard::PutString(allianceKey, redAllianceString);
	else if (alliance == DriverStation::Alliance::kBlue)
		SmartDashboard::PutString(allianceKey, blueAllianceString);
	else
		SmartDashboard::PutString(allianceKey, undefinedAllianceString);
	
	drivetrainInitialTelemetry();
}

void DriveTrain::sendTelemetry() const {
	SmartDashboard::PutNumber(matchTimeKey, DriverStation::GetInstance().GetMatchTime());
	
	double pdpCurrent = 0;
	double pdpPower = 0;
	double pdpEnergy = 0;
	
	pdp->GetTotalCurrent(pdpCurrent);
	pdp->GetTotalPower(pdpPower);
	pdp->GetTotalEnergy(pdpEnergy);
	
	SmartDashboard::PutNumber(pdpTotalCurrentKey, pdpCurrent);
	SmartDashboard::PutNumber(pdpTotalPowerKey, pdpPower);
	SmartDashboard::PutNumber(pdpTotalEnergyKey, pdpEnergy);
	
	SmartDashboard::PutNumber(controllerVoltageKey, ControllerPower::GetInputVoltage());
	SmartDashboard::PutNumber(controllerCurrentKey, ControllerPower::GetInputCurrent());
	
	double controllerVoltageFaults =
			ControllerPower::GetFaultCount3V3() +
			ControllerPower::GetFaultCount5V() +
			ControllerPower::GetFaultCount6V();
	
	SmartDashboard::PutNumber(totalControllerVoltageFaultsKey, controllerVoltageFaults);
	
	drivetrainTelemetry();
}

void DriveTrain::stop() {
	setLeftMotorsSpeed(0);
	setRightMotorsSpeed(0);
}
