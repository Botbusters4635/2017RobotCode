//
// Created by Alberto on 11/26/2017.
//

#include "PurePursuitRunner.h"

PurePursuitRunner::PurePursuitRunner(Path &path, double lookAheadDistance) : PathRunner(path) {
	
	currentTargetWaypoint = 0;
	this->lookAheadDistance = lookAheadDistance;
}

void PurePursuitRunner::runPath(DriveTrain &driveTrain, double speed) {
	if(!started){
		log->info("Started running path: {} on: {} ", currentPath.getName(),  driveTrain.getName());
		started = true;
	}
	TankState &state = TankState::getInstance();
	if (speed > driveTrain.getMaxSpeed()) {
		speed = EctoUtils::map(speed, 0.0, speed, 0.0,
		                       driveTrain.getMaxSpeed());
	}
	
	if (currentTargetWaypoint + 1 < currentPath.trajectorySize()) {
		finished = false;
		Twist2d &lastState = *state.getLastState();
		
		Waypoint &targetWaypoint = *currentPath.getWaypoint(currentTargetWaypoint);
		
		double distanceTarget = Point::getDistancePoints(lastState, targetWaypoint);
		
		while (distanceTarget < lookAheadDistance && currentTargetWaypoint + 1 < currentPath.trajectorySize()) {
			currentTargetWaypoint++;
			targetWaypoint = *currentPath.getWaypoint(currentTargetWaypoint);
			distanceTarget = Point::getDistancePoints(lastState, targetWaypoint);
		}
		
		double angleToTarget = Point::getAngleToPoint(lastState, targetWaypoint);
		
		double robotAngle = driveTrain.getYaw();
		double angleToTurn = EctoUtils::shortestAngleTurn(robotAngle, angleToTarget);
		driveTrain.driveFollowingAngle(speed, driveTrain.getYaw() + angleToTurn);
		
	} else {
		Twist2d &lastState = *state.getLastState();
		double distanceTarget = Point::getDistancePoints(lastState, *currentPath.getWaypoint(currentPath.trajectorySize()));
		double robotAngle = driveTrain.getYaw();
		double angleToTarget = Point::getAngleToPoint(lastState, *currentPath.getWaypoint(currentPath.trajectorySize()));
		double angleToTurn = EctoUtils::shortestAngleTurn(robotAngle, angleToTarget);
		if(distanceTarget > 0 && !finished)
			driveTrain.driveFollowingAngle(speed, driveTrain.getYaw() + angleToTurn);
		else {
			finished = true;
			driveTrain.stop();
		}
		
	}
}

bool PurePursuitRunner::isFinished() {
	return finished;
}
