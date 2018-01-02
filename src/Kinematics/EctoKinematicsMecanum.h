#ifndef BOTBUSTERSREBIRTH_ECTOKINEMATICSMECANUM_H
#define BOTBUSTERSREBIRTH_ECTOKINEMATICSMECANUM_H

#include "Utilities/EctoUtils.h"
#include "Utilities/MotorValues.h"
#include "Math/Twist2d.h"
#include <memory>
#include <cmath>
#include "EctoKinematics.h"
#include "Utilities/EctoSettings.h"

class EctoKinematicsMecanum final : EctoKinematics {
public:
	static EctoKinematicsMecanum &getInstance() {
		static EctoKinematicsMecanum singletonInstance;
		return singletonInstance;
	}
	
	shared_ptr<MotorValues> inverseKinematics(double vX, double vY, double rotation) override;
	
	shared_ptr<MotorValues> inverseKinematics(double vX, double vY, double rotation, double radius) override;
	
	shared_ptr<MotorValues> inverseKinematics(const Twist2d &in) override;
	
	shared_ptr<MotorValues> inverseKinematics(const Twist2d &in, double radius) override;
	
	shared_ptr<Twist2d> inverseKinematicsField(double vX, double vY, double gyro);
	
	shared_ptr<Twist2d> inverseKinematicsField(const Twist2d &in);
	
	const shared_ptr<Twist2d> kinematics(shared_ptr<MotorValues> in) override;
	
	const shared_ptr<Twist2d> kinematics(shared_ptr<MotorValues> in, double radius) override;

private:
	EctoKinematicsMecanum();
	
	EctoKinematicsMecanum &operator=(const EctoKinematicsMecanum &);
	
	EctoSettings &settings = EctoSettings::getInstance();
	
	double lX, lY;
	double lSum;
};

#endif
