/*
 * EctoPath.h
 *
 *  Created on: Jan 29, 2017
 *      Author: abiel
 */

#ifndef SRC_SUBSYSTEMS_ECTOPATH_ECTOPATHOPTIMIZE_H_
#define SRC_SUBSYSTEMS_ECTOPATH_ECTOPATHOPTIMIZE_H_

#include <vector>
#include <iostream>
#include <sstream>
#include "omp.h"

using std::vector;
using std::string;

using path = vector<vector<double>>;
using singlePath = vector<double>;



class EctoPathOptimize {
public:
	EctoPathOptimize(path InPath);

	string ParseToString(singlePath InPath);

	double RadiansToDegrees(double radians);

	path Inject(path orig, int numToInject);

	path InitializePath(path in);

	path Smoother(path pathIn, double weight_data, double weight_smooth,double tolerance);

	path NodeOnlyWayPoints(path pathIn);

	singlePath errorSum(path origVelocity, path smoothVelocity);

	path velocityFix(path smoothVelocity, path origVelocity, double tolerance);

	path velocity(path smoothPath, double timeStep);

	vector<int> injectionCounter2Steps(double numNodeOnlyPoints, double maxTimeToComplete, double timeStep);

	void leftRight(path smoothPath, double robotTrackWidth);

	singlePath getXVector(path arr);

	singlePath getYVector(path arr);

	path ResizePath(path inPath, unsigned int x, unsigned int y);

	path transposeVector(path pathIn);

	void setPathAlpha(double alpha);

	path smoother(path pathIn, double weight_data, double weight_smooth,double tolerance);

	void setPathBeta(double beta);

	void setPathTolerance(double tolerance);

	void calculate(double totalTime, double timeStep, double robotTrackWidth);

	void printPath(path inPath);

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

	double velocityAlpha = 0.1;
	double velocityBeta = 0.3;
	double velocityTolerance = 0.0000001;
	const double PI = 3.14; //TODO Accurate PI

	double totalTime;
	double totalDistance;
	double numFinalPoints;

	virtual ~EctoPathOptimize();

};



#endif /* SRC_SUBSYSTEMS_ECTOPATH_ECTOPATHOPTIMIZE_H_ */
