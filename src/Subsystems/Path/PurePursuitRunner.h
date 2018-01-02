//
// Created by Alberto on 11/26/2017.
//

#ifndef BOTBUSTERSREBIRTH_PUREPURSUITRUNNER_H
#define BOTBUSTERSREBIRTH_PUREPURSUITRUNNER_H

#include "PathRunner.h"
#include "Utilities/EctoUtils.h"
#include "Subsystems/Telemetry/TankState.h"
#include "spdlog/spdlog.h"

namespace spd = spdlog;

class PurePursuitRunner : public PathRunner {

public:
	explicit PurePursuitRunner(Path &path, double lookAheadDistance = 1);
	
	void runPath(DriveTrain &driveTrain, double speed) override;
	
	bool isFinished();

private:
	shared_ptr<spd::logger> log = spd::get("log");
	bool started, finished;
	double lookAheadDistance;
	unsigned int currentTargetWaypoint;
	
	
};

#endif //BOTBUSTERSREBIRTH_PUREPURSUITRUNNER_H
