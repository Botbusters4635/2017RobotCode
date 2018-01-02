#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "WPILib.h"
#include "DriverStation.h"
#include "ctrlib/CtreCanNode.h"
#include "ctre/PDP.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/MotorValues.h"
#include "Utilities/EctoUtils.h"
#include "Subsystems/EctoSubsystem.h"

class DriveTrain : public EctoSubsystem {
public:
	DriveTrain(const string &name);
	
	virtual void writeMotors(shared_ptr<MotorValues> in) = 0;
	
	virtual void setRightMotorsSpeed(double speed) = 0;
	
	virtual void setLeftMotorsSpeed(double speed) = 0;
	
	virtual double getRightMotorsSpeed() const = 0;
	
	virtual double getLeftMotorsSpeed() const = 0;
	
	virtual void driveFollowingAngle(double speed, double angle);
	
	virtual bool isAtAngle(double angle);
	
	virtual void turnToAngle(double angle);
	
	virtual void setBrakes(bool state) = 0;
	
	virtual void stop();
	
	virtual double getMaxSpeed() const = 0;
	
	virtual double getMinSpeed() const = 0;
	
	virtual double getYaw() = 0;
	
	void sendInitialTelemetry() const override;
	
	void sendTelemetry() const override;

protected:
	
	virtual void drivetrainInitialTelemetry() const = 0;
	
	virtual void drivetrainTelemetry() const = 0;

private:
	const double maxErrorAngle = 20.0;
	const double angleErrorThreshold = 0.5;
	
	EctoSettings &settings = EctoSettings::getInstance();
	
	uint8_t pdpId;
	
	shared_ptr<PDP> pdp;
	
	static constexpr auto redAllianceString = "Red Alliance";
	static constexpr auto blueAllianceString = "Blue Alliance";
	static constexpr auto undefinedAllianceString = "Undefined Alliance";
	
	static constexpr auto allianceKey = "Alliance";
	static constexpr auto matchTimeKey = "MatchTime";
	
	static constexpr auto pdpTotalCurrentKey = "PDP Current"; //Amps
	static constexpr auto pdpTotalPowerKey = "PDP Power"; //Watts
	static constexpr auto pdpTotalEnergyKey = "PDP Energy"; //Joules
	
	static constexpr auto controllerVoltageKey = "Controller Voltage";
	static constexpr auto controllerCurrentKey = "Controller Current";
	static constexpr auto totalControllerVoltageFaultsKey = "Controller Voltage Faults";
	
};

#endif