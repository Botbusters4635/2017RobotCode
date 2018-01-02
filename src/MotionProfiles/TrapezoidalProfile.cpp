//
// Created by abiel on 11/25/17.
//

#include "TrapezoidalProfile.h"

TrapezoidalProfile::TrapezoidalProfile(double time, double distance, double acceleration, double maxVelocity,
                                       double timeStep, double initialVelocity) {
	this->requestedTime = time;
	this->distance = distance;
	this->acceleration = acceleration;
	this->maxVelocity = maxVelocity;
	this->timeStep = timeStep;
	this->initialVelocity = initialVelocity;
	
	//Calculates velocity to cruise at
	cruiseVelocity = min(maxVelocity, acceleration * (time / 2.0) + initialVelocity);
	
	//Calculates how much time will it take to reach cruise velocity
	accelTime = cruiseVelocity / acceleration;
	
	//Calculates how much distance will it move to reach cruise velocity
	accelDistance = (.5 * acceleration) * pow(accelTime, 2.0);
	
	//Calculates remaining cruise velocity
	cruiseDistance = distance - 2.0 * (accelDistance);
	cruiseDistance = cruiseDistance > 0 ? cruiseDistance : 0;
	
	//Calculates time at which to cruise at
	cruiseTime = cruiseDistance / cruiseVelocity;
	
	//Calculates time at which to decelerate at
	decelTime = cruiseTime + accelTime;
	
	//Calculates total time
	totalTime = accelTime * 2.0 + cruiseTime;
	
	if (totalTime != time) {
		cout << "Could not perform profile at requested time! Running at: " + to_string(totalTime) << endl;
	}
	
	totalPoints = this->totalTime / this->timeStep;
}

double TrapezoidalProfile::getVelocity(double time) {
	double velocity = 0;
	
	if (time <= accelTime) {
		//If it is in the acceleration period
		velocity = acceleration * time;
	} else if (time > accelTime && time < decelTime) {
		//If it is in the cruise period
		velocity = cruiseVelocity;
	} else if (time >= decelTime) {
		//If it is in the deceleration period
		velocity = cruiseVelocity - acceleration * (time - decelTime);
	} else {
		throw logic_error("Invalid time: " + to_string(time));
	}
	
	//Changes super small values to 0
	velocity = velocity > epsilon ? velocity : 0;
	
	return velocity;
}

double TrapezoidalProfile::calculatePosition(double previousVelocity, double currentVelocity, double timeStep) {
	return ((previousVelocity + currentVelocity) / 2) * timeStep;
}

void TrapezoidalProfile::generateProfile() {
	shared_ptr<MotionProfilePoint> initialPoint = make_shared<MotionProfilePoint>(0, 0, 0);
	
	//data.emplace_back(initialPoint);
	
	for (unsigned int i = 0; i <= this->totalPoints; ++i) {
		shared_ptr<MotionProfilePoint> temp = make_shared<MotionProfilePoint>(0, 0, 0);
		
		double time = i * this->timeStep;
		
		bool invalidValue = i == 0;
		
		double previousValue = i != 0 ? i - 1 : 0;
		
		double previousVelocity = 0;
		
		if (!invalidValue)
			previousVelocity = data[previousValue]->velocity;
		else
			previousVelocity = 0;
		
		temp->velocity = this->getVelocity(time);
		
		temp->position = this->calculatePosition(previousVelocity, temp->velocity, this->timeStep);
		
		temp->duration = this->timeStep;
		
		data.emplace_back(temp);
	}
	
	isGenerated = true;
}

void TrapezoidalProfile::printValues() {
	cout << "Cruise Velocity: " << cruiseVelocity << endl;
	cout << "Accel Time: " << accelTime << endl;
	cout << "Accel Distance: " << accelDistance << endl;
	cout << "Cruise Distance: " << cruiseDistance << endl;
	cout << "Cruise Time: " << cruiseTime << endl;
	cout << "Decel Time: " << decelTime << endl;
	cout << "Total Time: " << totalTime << endl;
}

