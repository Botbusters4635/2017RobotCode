/*
* Settings
* Group: Mecanum
* StrafeExpo: Expo parameter for strafing.
* ForwardExpo: Expo for forwards movement.
* RotExpo: Exponent for rotation.
*
* P_Rot: P Value for Rotation Rate Loop
* I_Rot: I Value for Rotation Rate Loop
* D_Rot: D Value for Rotation Rate Loop
* Tol_Rot: Tolerance Value for Rotation Rate Loop
*
* MotorP: P Values for motors
* MotorI: I Values for motors
* MotorD: D Values for motors
* MotorF: F Values for Motors
*
* RotationRate: Maximum Rotation Rate
* MaxSpeed: Max RPM for movement
*/

#ifndef BOTBUSTERSREBIRTH_ECTOMECANUM_H
#define BOTBUSTERSREBIRTH_ECTOMECANUM_H

#include "Controllers/EctoTalon.h"
#include "Utilities/MotorValues.h"
#include "Utilities/EctoSettings.h"
#include "Subsystems/DriveTrain/DriveTrain.h"
#include "Math/Twist2d.h"
#include "Kinematics/EctoKinematicsMecanum.h"
#include "PIDs/NavXYawPID.h"
#include "Sensors/NavX.h"
#include <memory>
#include <cmath>
#include <PIDController.h>
#include "Subsystems/Manager/EctoInput.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "WPILib.h"

using namespace std;

class EctoMecanum : public DriveTrain {
public:
	static EctoMecanum &getInstance() {
		static EctoMecanum singletonInstance;
		return singletonInstance;
	}
	
	void update();
	
	void writeMotors(shared_ptr<MotorValues> in) override;
	
	void initTeleop() override;
	
	void updateTeleop() override;
	
	void initAuto() override;
	
	void updateAuto() override;
	
	void initDisabled() override;
	
	void updateDisabled() override;
	
	void setRightMotorsSpeed(double speed) override;
	
	void setLeftMotorsSpeed(double speed) override;
	
	double getRightMotorsSpeed() const override;
	
	double getLeftMotorsSpeed() const override;
	
	void setBrakes(bool state) override;
	
	double getYaw() override;
	
	double getMaxSpeed() const override;
	
	double getMinSpeed() const override;
	
	void drivetrainInitialTelemetry() const override;
	
	void drivetrainTelemetry() const override;
	
	double xOut, yOut;
private:
	EctoMecanum();
	
	EctoMecanum &operator=(const EctoMecanum &);
	
	NavX &gyro = NavX::getInstance();
	
	EctoSettings &settings = EctoSettings::getInstance();
	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	
	EctoKinematicsMecanum &kinematics = EctoKinematicsMecanum::getInstance();
	
	EctoUtils::MotorIdsMecanum ids;
	
	EctoTalon &frontLeft, &frontRight;
	EctoTalon &backLeft, &backRight;
	
	void setupPIDs();
	
	void configPIDs(unsigned int profileSlot);
	
	NavXYawPID *sourcePID;
	PIDOutput *outputPID;
	
	PIDController *controller;
	
	double strafeExpo, forwardsExpo, rotExpo;
	
	double pRot, iRot, dRot, tolRot;
	
	double rotRate;
	
	double maximumSpeed;
};

#endif
