//
// Created by Abiel on 11/25/17.
//

#ifndef BOTBUSTERSREBIRTH_TRAPEZOIDALPROFILE_H
#define BOTBUSTERSREBIRTH_TRAPEZOIDALPROFILE_H

#include "MotionProfile.h"
#include "MotionProfilePoint.h"
#include <memory>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include "MotionProfile.h"
#include <cassert>
#include <vector>

/*
 * Generates a Trapezoidal Profile
 */

using namespace std;

class TrapezoidalProfile : public MotionProfile {
public:
	TrapezoidalProfile(double time, double distance, double acceleration, double maxVelocity, double timeStep = 0.01,
	                   double initialVelocity = 0);
	
	void generateProfile() override;
	
	double getVelocity(double time) override;
	
	void printValues();

private:
	double calculatePosition(double previousVelocity, double currentVelocity, double timeStep) override;
	
	double cruiseVelocity, cruiseTime, cruiseDistance;
	
};

#endif //BOTBUSTERSREBIRTH_TRAPEZOIDALPROFILE_H
