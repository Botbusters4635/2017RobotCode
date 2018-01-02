//
// Created by Abiel on 11/26/17.
//

#ifndef BOTBUSTERSREBIRTH_MOTIONPROFILEEXECUTER_H
#define BOTBUSTERSREBIRTH_MOTIONPROFILEEXECUTER_H

#include <Notifier.h>
#include "spdlog/spdlog.h"
#include "Controllers/EctoTalon.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "Utilities/EctoUtils.h"
#include "MotionProfile.h"
#include "MotionProfilePoint.h"
#include <memory>
#include <vector>
#include <string>
#include "ctrlib/CANTalon.h"
#include <thread>

using namespace std;

namespace spd = spdlog;

class MotionProfileExecuter {
public:
	MotionProfileExecuter(const shared_ptr<MotionProfile> &in, const string &motorName, unsigned int profileSlot,
	                      bool velocityOnly = true);
	
	void reset();
	
	void clearMotionProfile();
	
	void controlTalon();
	
	void setStart(bool in);
	
	enum executerStatus {
		notFilled = 0,
		filling = 1,
		stopped = 2,
		running = 3,
		finished = 4
	};


private:
	EctoMotors &motors = EctoMotors::getInstance();
	
	shared_ptr<spd::logger> log = spd::get("log");
	
	CANSpeedController::ControlMode previousControlMode;
	
	void setupTalon();
	
	void sendToTalon();
	
	void periodicTask();
	
	CANTalon::TrajectoryPoint
	convertToTrajectoryPoint(shared_ptr<MotionProfilePoint> in, bool firstPoint, bool lastPoint);
	
	shared_ptr<MotionProfile> profile;
	
	string motorName;
	
	EctoTalon &motor;
	
	
	bool velocityOnly;
	
	unsigned int profileSlot;
	
	bool start = false;
	bool firstFilling = true;
	
	CANTalon::MotionProfileStatus talonStatus;
	CANTalon::SetValueMotionProfile setValue = CANTalon::SetValueMotionProfileDisable;
	
	//Minimum of points to load into talon before starting motion profile
	const unsigned int minimumPoints = 1;
	
	executerStatus status = notFilled;
	
	shared_ptr<Notifier> periodicNotifier;
	
	double index;
};

#endif //BOTBUSTERSREBIRTH_MOTIONPROFILEEXECUTER_H
