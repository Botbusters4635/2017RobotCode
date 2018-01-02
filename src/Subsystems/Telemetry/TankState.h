/*
* Settings
* Group: EctoState
* SideQuantity: Amount of motors per side
* WheelRadius: Wheel Radius in meters
* Motor1L - Motor#L: Left Motors IDs
* MotorInvert1L - MotorInvert#L: Left Motor invert status
* Motor1R - Motor#R: Right Motors IDs
* MotorInvert1R - MotorInvert#R: Right Motor invert status
 */

#ifndef BOTBUSTERSREBIRTH_ECTOROBOTSTATE_H
#define BOTBUSTERSREBIRTH_ECTOROBOTSTATE_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "Subsystems/DriveTrain/EctoMecanum.h"
#include "Math/Twist2d.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <map>
#include <utility>
#include <memory>
#include <thread>
#include "Sensors/NavX.h"

using namespace std;
typedef chrono::steady_clock Time;
typedef chrono::milliseconds ms;
typedef chrono::duration<float> fsec;


class vec {
public:
	double mag, angle;
};

/**
 * Calculates the current position of the robot using encoders and a gyroscope.
 * @return
 */

class TankState final {
public:
	static TankState &getInstance() {
		static TankState singletonInstance;
		return singletonInstance;
	}
	TankState(TankState& rhs) = delete;
	TankState &operator=(const TankState &) = delete;
	
	double getDistanceTraveled();
	
	shared_ptr<Twist2d> getLastState();
	
	void start();
	
	void stop();


private:
	//Total accumulated position vector
	vec sum;
	thread threadUpdate;
	bool isEnabled = false;
	
	double lastTime = 0.0;
	double lastDistance = 0.0, currentDistance = 0.0;
	EctoSettings &settings = EctoSettings::getInstance();
	EctoMotors &motors = EctoMotors::getInstance();
	NavX &navx = NavX::getInstance();
	//XRS450 &gyro = XRS450::getInstance(SPI::Port::kOnboardCS0);
	
	chrono::steady_clock::time_point startTime = Time::now();
	chrono::steady_clock::time_point nextTime = Time::now();
	
	TankState();
	
	
	double getLeftDistance();
	
	double getRightDistance();
	
	double getYawNavX();
	
	double getTime();
	
	void addPoint();
	
	void update();
	
	void printLastState();
	
	
	vector<shared_ptr<Twist2d>> values;
	
	vector<double> times;
	int maxLoggedStates;
	double wheelDiameter;
	//time counter is on seconds
	double stateUpdateSec;
	double threadUpdateSec;
	
	double gyroTresh;
	double gyroPrevValue;
	
	
	double rDistance = 0, lDistance = 0;
	
};

#endif
