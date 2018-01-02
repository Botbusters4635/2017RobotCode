//
// Created by abiel on 11/29/17.
//

#ifndef BOTBUSTERSREBIRTH_FEEDFORWARDCALCULATOR_H
#define BOTBUSTERSREBIRTH_FEEDFORWARDCALCULATOR_H

#include <Timer.h>
#include "Controllers/EctoTalon.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "EctoUtils.h"
#include <string>
#include "ctrlib/CANTalon.h"
#include "spdlog/spdlog.h"

using namespace std;
namespace spd = spdlog;

class FeedForwardCalculator final {
public:
	
	static FeedForwardCalculator &getInstance() {
		static FeedForwardCalculator singletonInstance;
		return singletonInstance;
	}
	
	void setMotor(string motorName, uint16_t codesPerRev = 480);
	
	bool periodic();
	
	double getMaxSpeedRPM();
	
	double getMaxNativeSpeed();
	
	double getFgain();
	
	void printValues();

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("FeedForwardCalculator");

	FeedForwardCalculator();
	
	FeedForwardCalculator &operator=(const FeedForwardCalculator &);
	
	EctoMotors motors = EctoMotors::getInstance();
	
	CANTalon::TalonControlMode previousControlMode;
	
	void setMaxSpeed();
	
	double getMaxSpeed();
	
	double calculateFgain();
	
	void resetTalon();
	
	string motorName;
	
	shared_ptr<EctoTalon> motor;
	
	uint16_t codesPerRev = 0;
	
	double maxSpeed;
	
	double fGain;
	
	shared_ptr<Timer> timer;
	
	bool running = false;
	bool finished = false;
	bool started = false;
	
	const double maxTalonSpeed = 1;
	const double timeToWaitToAccelerate = 1.0;
	const double talonMaxRequestedSpeed = 1023;
};

#endif //BOTBUSTERSREBIRTH_FEEDFORWARDCALCULATOR_H
