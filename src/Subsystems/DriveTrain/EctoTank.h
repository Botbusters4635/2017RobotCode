/*
 * Settings
 * Group: Tank
 * MaxSpeed_Speed: Max speed for speed mode.
 * MaxSpeed_Torque: Max speed for torque mode.
 * ExpoJoy: Expo value for joysticks.
 * ExpoTrig: Expo value for triggers.
 * Speed_P: P Value for Speed.
 * Speed_I: I Value for Speed.
 * Speed_D: D Value for Speed.
 * Speed_F: F Value for Speed.
 * Torque_P: P Value for Torque.
 * Torque_I: I Value for Torque.
 * Torque_D: D Value for Torque.
 * Torque_F: F Value for Torque.
 */
#ifndef BOTBUSTERSREBIRTH_ECTOTANK_H
#define BOTBUSTERSREBIRTH_ECTOTANK_H

#include "Subsystems/DriveTrain/DriveTrain.h"
#include "Subsystems/Manager/EctoInput.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "Subsystems/Manager/EctoPCM.h"
#include "Controllers/EctoTalon.h"
#include "Sensors/NavX.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/MotorValues.h"
#include <SmartDashboard/SmartDashboard.h>
#include <SmartDashboard/SendableChooser.h>
#include "Subsystems/Path/PathParser.h"
#include "Subsystems/Path/PurePursuitRunner.h"
#include <string>
#include <memory>
#include <chrono>
#include <cmath>

typedef chrono::steady_clock Time;
typedef chrono::milliseconds ms;
typedef chrono::duration<float> fsec;

enum TankDriveMode {
	CurveDrive,
	TankDrive
};

class EctoTank : public DriveTrain {
public:
	static EctoTank &getInstance() {
		static EctoTank singletonInstance;
		return singletonInstance;
	}
	void updateSettings();
	
	void writeMotors(shared_ptr<MotorValues> in) override;
	
	bool isSpeedShift() const;
	
	bool isSpeedPistonEnabled() const;
	
	void toggleShift();
	
	void enableShift();
	
	void disableShift();
	
	void toggleInverted();
	
	void toggleQuickTurn();
	
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
	
	TankDriveMode getDriveMode();
	
	void setDriveMode(TankDriveMode mode);

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("EctoTank");
	
	PathParser parser = PathParser("/home/lvuser/paths");
	void updatePaths();
	shared_ptr<Path> path;
	shared_ptr<PurePursuitRunner> runner;
	DigitalOutput *speedLedFeedback;
	SendableChooser<string> *pathChooser;
	
	EctoTank();
	
	EctoTank &operator=(const EctoTank &);
	
	NavX &navx = NavX::getInstance();
	
	EctoSettings &settings = EctoSettings::getInstance();
	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	EctoPCM &pcm = EctoPCM::getInstance();
	
	double voltageRampRate;
	
	void setupSlaves();
	
	void setupPids();
	
	EctoTalon &leftMaster, &rightMaster;
	
	vector<shared_ptr<EctoTalon>> leftSlaves, rightSlaves;
	
	double expoJoy, expoTrig;
	
	double maxSpeed_Speed, maxSpeed_Torque;
	
	double minSpeedShiftEnabled;
	
	EctoUtils::PidValues speedPid;
	EctoUtils::PidValues torquePid;
	
	double wheelDiameter;
	
	const double alpha = 0.1;
	double quickStopAccumulator;
	
	const bool brakeState = true;
	
	const unsigned int speedProfileSlot = 0;
	const unsigned int torqueProfileSlot = 1;
	
	unsigned int currentProfile = 0;
	
	bool speedShift = false;
	
	bool speedPistonEnabled = false;
	chrono::steady_clock::time_point lastSpeedChange = Time::now();
	bool isInSpeedTimeout = false;
	double speedChangeTimeoutSec = .75;
	
	bool inverted;
	bool quickTurn;
	bool lastX = false, lastA = false, lastY = false;
	
	void selectProfileSlots(unsigned int profileSlot);
	
	void setSpeedMode();
	
	void setTorqueMode();
	
	void drivetrainInitialTelemetry() const override;
	
	void drivetrainTelemetry() const override;
	
	TankDriveMode currentMode = TankDrive;
	
	shared_ptr<MotorValues> tankDrive(bool inverted);
	
	shared_ptr<MotorValues> curveDriveSingleJoy(bool isQuickTurn);
	
	shared_ptr<MotorValues> curveDriveWheel(bool isQuickTurn, bool invert);
	
	shared_ptr<MotorValues> curveDrive(double y, double rotation, bool isQuickTurn);
};

#endif //BOTBUSTERSREBIRTH_ECTOTANK2_H
