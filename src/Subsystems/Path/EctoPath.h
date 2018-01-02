/*
 * EctoPath.h
 *
 *  Created on: Jan 29, 2017
 *      Author: abiel
 */

#ifndef BOTBUSTERSREBIRTH_SUBSYSTEMS_ECTOPATH_ECTOPATH_H_
#define BOTBUSTERSREBIRTH_SUBSYSTEMS_ECTOPATH_ECTOPATH_H_

#include <vector>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cmath>
#include <cmath>
#include <IterativeRobot.h>

using namespace std;

using path = vector<vector<double> >;
using singlePath = vector<double>;

/**
 * Generates smooth paths used for autonomous control
 * @param InPath         Input Path
 * @param motorReduction Ammount of motor reduction
 */
class EctoPath {

public:
	
	EctoPath(path InPath, double motorReduction = 0);
	
	string ParseToString(singlePath InPath);
	
	double RadiansToDegrees(double radians);
	
	double deegrestoRadian(double deegres);
	
	path Inject(path orig, int numToInject);
	
	path InitializePath(path in);
	
	path Smoother(path pathIn, double weight_data, double weight_smooth,
	              double tolerance);
	
	path NodeOnlyWayPoints(path pathIn);
	
	singlePath errorSum(path origVelocity, path smoothVelocity);
	
	path velocityFix(path smoothVelocity, path origVelocity, double tolerance);
	
	path velocity(path smoothPath, double timeStep);
	
	vector<int> injectionCounter2Steps(double numNodeOnlyPoints,
	                                   double maxTimeToComplete,
	                                   double timeStep);
	
	void leftRight(path smoothPath, double robotTrackWidth);
	
	singlePath getXVector(path arr);
	
	singlePath getYVector(path arr);
	
	path ResizePath(path inPath, unsigned int x, unsigned int y);
	
	path transposeVector(path pathIn);
	
	void setPathAlpha(double alpha);
	
	path smoother(path pathIn, double weight_data, double weight_smooth,
	              double tolerance);
	
	void setPathBeta(double beta);
	
	void setPathTolerance(double tolerance);
	
	void calculate(double totalTime, double timeStep, double robotTrackWidth);
	
	void printPath(path inPath);
	
	long speedToRPM(double speed);
	
	void setWheelDiameter(double diameter);
	
	double getWheelDiameter();
	
	void setGearReduction(double reduction);
	
	double getGearReduction();
	
	path origPath;
	path nodeOnlyPath;
	path smoothPath;
	path leftPath;
	path rightPath;

//Orig Velocity
	path origCenterVelocity;
	path origRightVelocity;
	path origLeftVelocity;

//smooth velocity
	path smoothCenterVelocity;
	path smoothRightVelocity;
	path smoothLeftVelocity;
	
	path heading;

//Magic Values
	double pathAlpha = 0.7;
	double pathBeta = 0.3;
	double pathTolerance = 0.0000001;
	
	double velocityAlpha = 0.7;
	double velocityBeta = 0.3;
	double velocityTolerance = 0.0000001;
//const double PI = M_PI;
	
	double totalTime;
	double totalDistance;
	double numFinalPoints;
	
	double timeStep;
	bool calculated = false;
	
	virtual ~EctoPath();
	
};

#endif /* BOTBUSTERSREBIRTH_SUBSYSTEMS_ECTOPATH_ECTOPATH_H_ */
