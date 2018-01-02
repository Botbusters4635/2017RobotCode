//
// Created by alber on 12/6/2017.
//

#ifndef BOTBUSTERSREBIRTH_ECTOROBOT_H
#define BOTBUSTERSREBIRTH_ECTOROBOT_H

#include "spdlog/spdlog.h"
#include <WPILib.h>
#include "Utilities/EctoSettings.h"
#include "Subsystems/DriveTrain/DriveTrain.h"
#include "Subsystems/DriveTrain/EctoTank.h"
#include "Subsystems/DriveTrain/EctoMecanum.h"
#include "EctoSubsystem.h"

namespace spd = spdlog;

class EctoRobot : public IterativeRobot {
public:
	EctoRobot();

protected:
	DriveTrain *driveTrain;
	SubsystemManager *manager;
	string driveTrainSetting;
	EctoSettings &settings = EctoSettings::getInstance();

private:
	std::shared_ptr<spdlog::logger> log;
	
	
};


#endif //BOTBUSTERSREBIRTH_ECTOROBOT_H
