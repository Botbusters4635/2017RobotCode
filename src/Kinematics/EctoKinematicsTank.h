/*
* Settings
* Group: Tank
* WheelWidth: Wheel Width
* TankScrubFactor: ???
*/

#ifndef BOTBUSTERSREBIRTH_ECTOKINEMATICSTANK_H
#define BOTBUSTERSREBIRTH_ECTOKINEMATICSTANK_H

#include "Math/Twist2d.h"
#include "Utilities/MotorValues.h"
#include "EctoKinematics.h"
#include "Utilities/EctoSettings.h"
#include <memory>

class EctoKinematicsTank final : EctoKinematics {
public:
	static EctoKinematicsTank &getInstance() {
		static EctoKinematicsTank singletonInstance;
		return singletonInstance;
	}
	
	shared_ptr<MotorValues> inverseKinematics(double vX, double vY, double rotation) override;
	
	shared_ptr<MotorValues> inverseKinematics(double vX, double vY, double rotation, double radius) override;
	
	shared_ptr<MotorValues> inverseKinematics(const Twist2d &in) override;
	
	shared_ptr<MotorValues> inverseKinematics(const Twist2d &in, double radius) override;
	
	const shared_ptr<Twist2d> kinematics(shared_ptr<MotorValues> in) override;
	
	const shared_ptr<Twist2d> kinematics(shared_ptr<MotorValues> in, double radius) override;

private:
	EctoKinematicsTank();
	
	EctoKinematicsTank &operator=(const EctoKinematicsTank &);
	
	EctoSettings &settings = EctoSettings::getInstance();
	
	double wheelWidth = 0;
	double tankScrubFactor = 0;
};

#endif
