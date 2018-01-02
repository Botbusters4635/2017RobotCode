//
// Created by abiel on 11/26/17.
//

#ifndef BOTBUSTERSREBIRTH_PROFILE_H
#define BOTBUSTERSREBIRTH_PROFILE_H

#include "MotionProfilePoint.h"
#include <memory>
#include <vector>

using namespace std;

class MotionProfile {
public:
	virtual void generateProfile() = 0;
	
	virtual double getVelocity(double time) = 0;

protected:
	
	double totalTime;
	
	double timeStep;
	
	vector<shared_ptr<MotionProfilePoint>> data;
	
	unsigned int totalPoints;
	const double epsilon = 1e-6;
	
	double requestedTime, distance, acceleration, maxVelocity, initialVelocity;
	
	double accelTime, accelDistance, decelTime;
	
	bool isGenerated = false;
	
	virtual double calculatePosition(double previousVelocity, double currentVelocity, double timeStep) = 0;

public:
	double getTotalTime() const {
		return totalTime;
	}
	
	double getTimeStep() const {
		return timeStep;
	}
	
	const vector<shared_ptr<MotionProfilePoint>> &getData() const {
		return data;
	}
	
	unsigned int getTotalPoints() const {
		return totalPoints;
	}
	
	double getRequestedTime() const {
		return requestedTime;
	}
	
	double getDistance() const {
		return distance;
	}
	
	double getAcceleration() const {
		return acceleration;
	}
	
	double getMaxVelocity() const {
		return maxVelocity;
	}
	
	double getInitialVelocity() const {
		return initialVelocity;
	}
	
	double getAccelTime() const {
		return accelTime;
	}
	
	double getAccelDistance() const {
		return accelDistance;
	}
	
	double getDecelTime() const {
		return decelTime;
	}
	
	bool getGeneratedStatus() const {
		return isGenerated;
	}
};

#endif //BOTBUSTERSREBIRTH_PROFILE_H
