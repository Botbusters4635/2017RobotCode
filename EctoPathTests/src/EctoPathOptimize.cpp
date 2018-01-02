/*
 * EctoPath.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: abiel
 */

#include "EctoPathOptimize.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include "omp.h"

using std::vector;
using std::string;

using std::cout;
using std::endl;


using path = vector<vector<double> >;
using singlePath = vector<double>;


EctoPathOptimize::EctoPathOptimize(path InPath) {
	// TODO Auto-generated constructor stub
	//default values DO NOT MODIFY;
	origPath = InPath;
	pathAlpha = 0.7;
	pathBeta = 0.3;
	pathTolerance = 0.0000001;

	velocityAlpha = 0.1;
	velocityBeta = 0.3;
	velocityTolerance = 0.0000001;
}


//Works
string EctoPathOptimize::ParseToString(singlePath InPath) {
	std::ostringstream stringFormat;

	double multiplier = 1;
	//stringFormat << "X: ";
	stringFormat << InPath[0] * multiplier;
	stringFormat << ", ";
	stringFormat << InPath[1] * multiplier;
	stringFormat << ",";

	return stringFormat.str();
}


//Works
double EctoPathOptimize::RadiansToDegrees(double radians){
	return (radians * 180) / PI;
}


//Works
path EctoPathOptimize::ResizePath(path inPath, unsigned int x, unsigned int y){

	//resize vector of vector
	inPath.resize(x);

	//resize vectors inside of vector
#pragma omp parallel for
	for(unsigned int i = 0; i < inPath.size(); i++){
		inPath[i].resize(y);
	}

	return inPath;
}


//Works
path EctoPathOptimize::InitializePath(path in){
	in.resize(1);

	//#pragma omp for
	for (unsigned int i = 0; i < in.size(); i++){
		in[i].resize(2);
	}
	return in;
}

//Works
path EctoPathOptimize::Inject(path orig, int numToInject) {
//  path morePoints(orig.size() + ((numToInject)) * (orig.size() - 1), 2);
	path morePoints;
	path testPoints;

	//morePoints.resize(orig.size() + ((numToInject)) * (orig.size() - 1));
	morePoints = ResizePath(morePoints, orig.size() + ((numToInject) * (orig.size() - 1)), 2);

//;	path test(1,2);

	unsigned int index = 0;

#pragma omp parallel for
	for (unsigned int i = 0; i < orig.size() - 1; i++) {
		double test = orig[i][0];
		morePoints[index][0] = test;
		morePoints[index][1] = orig[i][1];

		index++;

		for (int j = 1; j < numToInject + 1; j++) {
			morePoints[index][0] = j
					* ((orig[i + 1][0] - orig[i][0]) / (numToInject + 1))
					+ orig[i][0];

			morePoints[index][1] = j
					* ((orig[i + 1][1] - orig[i][1]) / (numToInject + 1))
					+ orig[i][1];

			index++;
		}
	}

	morePoints[index][0] = orig[orig.size() - 1][0];
	morePoints[index][1] = orig[orig.size() - 1][1];

	index++;

	return morePoints;
}


//Maybe works?
path EctoPathOptimize::smoother(path pathIn, double weight_data, double weight_smooth,double tolerance){
	path newPath = pathIn;


	double change = tolerance;



	while(change >= tolerance){
		change = 0.0;


		for(unsigned int i = 1; i < pathIn.size() - 1; i++){
			#pragma omp for ordered schedule(dynamic)
			for(unsigned int j = 0; j < pathIn[i].size(); j++){
				double aux = newPath[i][j];
				//double calc = weight_data * (newPath[i][j] - newPath[i][j]) + weight_smooth * (newPath[i-1][j] + newPath[i+1][j] - (2.0 * newPath[i][j]));
				newPath[i][j] += weight_data * (pathIn[i][j] - newPath[i][j]) + weight_smooth * (newPath[i-1][j] + newPath[i+1][j] - (2.0 * newPath[i][j]));
				change += abs(aux - newPath[i][j]);
			}
		}
	}

	return newPath;
}

//TODO Check if this works
path EctoPathOptimize::NodeOnlyWayPoints(path pathIn){
	path li;

	li.push_back(pathIn[0]);

	#pragma omp for ordered schedule(dynamic)
	for(unsigned int i = 1; i < pathIn.size() - 1; i++){
		double vector1 = atan2((pathIn[i][1] - pathIn[i-1][1]), pathIn[i][0] - pathIn[i-1][0]);
		double vector2 = atan2((pathIn[i+1][1] - pathIn[i][1]), pathIn[i+1][0] - pathIn[i][0]);

		#pragma omp ordered
		if(abs(vector2 - vector1) >= 0.01){
			li.push_back(pathIn[i]);
		}
	}


	li.push_back(pathIn[pathIn.size() - 1]);

	path temp;
	//temp.resize(li.size());
	//temp = InitializePath(temp);

	temp = ResizePath(temp, li.size(), 2);



	for(unsigned int i = 0; i < li.size(); i++){
		temp[i][0] = li[i][0];
		temp[i][1] = li[i][1];
	}


	return temp;
}

path EctoPathOptimize::velocity(path smoothPath, double timeStep){
	unsigned int smoothPathSize = smoothPath.size();

	singlePath dxdt, dydt;
	dxdt.resize(smoothPathSize);
	dydt.resize(smoothPathSize);


	path velocity;

	velocity = ResizePath(velocity, smoothPathSize, 2);

	dxdt[0] = 0;
	dydt[0] = 0;

	velocity[0][0] = 0;
	velocity[0][1] = 0;

	heading[0][1] = 0;


	for(unsigned int i = 1; i < smoothPathSize; i++){
		dxdt[i] = (smoothPath[i][0] - smoothPath[i - 1][0]) / timeStep;
		dydt[i] = (smoothPath[i][1] - smoothPath[i - 1][1]) / timeStep;

		//create time vector

		velocity[i][0] = velocity[i - 1][0] + timeStep;
		heading[i][0] = heading[i - 1][0] + timeStep;

		//calculate velocity

		velocity[i][1] = sqrt(pow(dxdt[i], 2) + pow(dydt[i], 2));
	}


	return velocity;
}

path EctoPathOptimize::velocityFix(path smoothVelocity, path origVelocity, double tolerance){
	singlePath difference = errorSum(origVelocity, smoothVelocity);

	path fixVel;


	fixVel = ResizePath(fixVel, smoothVelocity.size(), 2);


	#pragma omp parallel for
	for(unsigned int i = 0; i < smoothVelocity.size(); i++){
		fixVel[i][0] = smoothVelocity[i][0];
		fixVel[i][1] = smoothVelocity[i][1];
	}

	double increase = 0.0;

	while(abs(difference[difference.size() - 1]) > tolerance){
		increase = difference[difference.size() - 1] / 1 / 50;


		#pragma omp simd
		for(unsigned int i = 1; i < fixVel.size(); i++){
			fixVel[i][1] = fixVel[i][1] - increase;
		}
		difference = errorSum(origVelocity, fixVel);
	}

	return fixVel;
}

singlePath EctoPathOptimize::errorSum(path origVelocity, path smoothVelocity){
	singlePath tempOrigDist, tempSmoothDist, difference;

	tempOrigDist.resize(origVelocity.size());
	tempSmoothDist.resize(smoothVelocity.size());
	difference.resize(smoothVelocity.size());

	double timeStep = origVelocity[1][0] - origVelocity[0][0];

	tempOrigDist[0] = origVelocity[0][1];
	tempSmoothDist[0] = smoothVelocity[0][1];


	#pragma omp parallel for
	for (unsigned int i = 1; i < origVelocity.size(); i++){
		tempOrigDist[i] = origVelocity[i][1] * timeStep + tempOrigDist[i - 1];
		tempSmoothDist[i] = smoothVelocity[i][1] * timeStep + tempSmoothDist[i - 1];

		difference[i] = tempSmoothDist[i] - tempOrigDist[i];
	}

	return difference;
}

vector<int> EctoPathOptimize::injectionCounter2Steps(double numNodeOnlyPoints, double maxTimeToComplete, double timeStep){
	int first = 0;
	int second = 0;
	int third = 0;

	cout << maxTimeToComplete/timeStep << endl;

	double oldPointsTotal = 0;

	double numFinalPoints = 0;

	vector<int> ret;

	double totalPoints = maxTimeToComplete/timeStep;


	if(totalPoints < 100){
		double pointsFirst = 0;
		double pointsTotal = 0;


		for(int i = 4; i <= 6; i++){
			for(int j = 1; j <= 8; j++){
				pointsFirst = i * (numNodeOnlyPoints - 1) + numNodeOnlyPoints;
				pointsTotal = (j*(pointsFirst - 1) + pointsFirst);

				if(pointsTotal <= totalPoints && pointsTotal > oldPointsTotal){
					first = i;
					second = j;
					numFinalPoints = pointsTotal;
					oldPointsTotal = pointsTotal;
				}
			}
		}
		ret.push_back(first);
		ret.push_back(second);
		ret.push_back(third);
	}
	else {
		double pointsFirst = 0;
		double pointsSecond = 0;
		double pointsTotal = 0;

		#pragma omp parallel for collapse (3)
		for(unsigned int i = 1; i <= 5; i++)
			for(unsigned int j = 1; j <= 8; j++)
				for(unsigned int k = 1; k < 8; k++){
					pointsFirst = i * (numNodeOnlyPoints - 1) + numNodeOnlyPoints;
					pointsSecond = (j * (pointsFirst - 1) + pointsFirst);
					pointsTotal = (k * (pointsSecond - 1) + pointsSecond);


					if(pointsTotal <= totalPoints){
						first = i;
						second = j;
						third = k;
						numFinalPoints = pointsTotal;
					}
				}
		ret.push_back(first);
		ret.push_back(second);
		ret.push_back(third);
	}

	return ret;
}

void EctoPathOptimize::leftRight(path smoothPath, double robotTrackWidth){
	unsigned int smoothPathSize = smoothPath.size();


	path leftPath2, rightPath2;

	leftPath2 = ResizePath(leftPath2, smoothPathSize, 2);
	rightPath2 = ResizePath(rightPath2, smoothPathSize, 2);

	path gradient;

	gradient = ResizePath(gradient, smoothPathSize, 2);

	unsigned int gradientSize = gradient.size();

	#pragma omp parallel for
	for(unsigned int i = 0; i < smoothPathSize - 1; i++)
		gradient[i][1] = atan2(smoothPath[i + 1][1] -smoothPath[i][1], smoothPath[i + 1][0] - smoothPath[i][0]);
	gradient[gradient.size() - 1][1] = gradient[gradient.size() - 2][1];

	#pragma omp parallel for
	for(unsigned int i = 0; i < gradientSize; i ++){
		leftPath2[i][0] = (robotTrackWidth / 2 * cos(gradient[i][1] + PI / 2)) + smoothPath[i][0];
		leftPath2[i][1] = (robotTrackWidth / 2 * sin(gradient[i][1] + PI / 2)) + smoothPath[i][1];

		rightPath2[i][0] = robotTrackWidth / 2 * cos(gradient[i][1] - PI / 2) + smoothPath[i][0];
		rightPath2[i][1] = robotTrackWidth / 2 * sin(gradient[i][1] - PI / 2) + smoothPath[i][1];

		double deg = RadiansToDegrees(gradient[i][1]);

		gradient[i][1] = deg;

		if(i > 0){
			if((deg - gradient[i - 1][1]) > 180)
				gradient[i][1] = -360 + deg;

			if((deg - gradient[i-1][1]) < -180)
				gradient[i][1] = 360 + deg;
		}
	}


	heading = gradient;
	leftPath = leftPath2;
	rightPath = rightPath2;
}

singlePath EctoPathOptimize::getXVector(path arr){
	singlePath temp;
	temp.resize(arr.size());

#pragma omp parallel for
	for(unsigned int i = 0; i < temp.size(); i++)
		temp[i] = arr [i][0];

	return temp;
}


singlePath EctoPathOptimize::getYVector(path arr){
	singlePath temp;
	temp.resize(arr.size());

#pragma omp parallel for
	for(unsigned int i = 0; i < temp.size(); i++)
		temp[i] = arr [i][1];

	return temp;
}

path EctoPathOptimize::transposeVector(path pathIn){
	path temp;
	ResizePath(temp, pathIn[0].size(), pathIn.size());

	for(unsigned int i = 0; i < temp.size(); i++)
		for(unsigned int j = 0; j < temp[i].size(); j++)
			temp[i][j] = pathIn[j][i];

	return temp;
}

//Looks weird, but that's how it's supposed to be... hopefully.

void EctoPathOptimize::setPathAlpha(double alpha){
	pathAlpha = alpha;
}

void EctoPathOptimize::setPathBeta(double beta){
		pathAlpha = beta;
}

void EctoPathOptimize::setPathTolerance(double tolerance){
		pathAlpha = tolerance;
}

void EctoPathOptimize::printPath(path inPath){

	for(unsigned int i = 0; i < inPath.size(); i++){
			string test = ParseToString(inPath[i]);
			cout << std::fixed << std::setprecision(32) << test << endl;
	}
}

/**
	 * This code will calculate a smooth path based on the program parameters. If the user doesn't set any parameters, the will use the defaults optimized for most cases. The results will be saved into the corresponding
	 * class members. The user can then access .smoothPath, .leftPath, .rightPath, .smoothCenterVelocity, .smoothRightVelocity, .smoothLeftVelocity as needed.
	 *
	 * After calling this method, the user only needs to pass .smoothRightVelocity[1], .smoothLeftVelocity[1] to the corresponding speed controllers on the Robot, and step through each setPoint.
	 *
	 * @param totalTime - time the user wishes to complete the path in seconds. (this is the maximum amount of time the robot is allowed to take to traverse the path.)
	 * @param timeStep - the frequency at which the robot controller is running on the robot.
	 * @param robotTrackWidth - distance between left and right side wheels of a skid steer chassis. Known as the track width.
	 */



void EctoPathOptimize::calculate(double totalTime, double timeStep, double robotTrackWidth){
	nodeOnlyPath = NodeOnlyWayPoints(origPath);





	vector<int> inject = injectionCounter2Steps(nodeOnlyPath.size(), totalTime, timeStep);

	cout << inject.size() << endl;

	//cout << inject.size() << endl;
	for(unsigned int i = 0; i < inject.size(); i++){
		if(i == 0){
			int temp = inject[0];
			smoothPath = Inject(nodeOnlyPath, temp);
			smoothPath = smoother(smoothPath, pathAlpha, pathBeta, pathTolerance);
			//smoothPath = smoother(smoothPath, 0.5, 0.9, pathTolerance);
			//printPath(smoothPath);
		}
		else{
			smoothPath = Inject(smoothPath, inject[i]);
			smoothPath = smoother(smoothPath, 0.1, 0.3, 0.0000001);
		}
	}


	//TODO test it

	//cout << smoothPath.size() << endl;

	//calculate left and right path based on center path
	leftRight(smoothPath, robotTrackWidth);

	//cout << "x" << endl;


	origCenterVelocity = velocity(smoothPath, timeStep);
	origLeftVelocity = velocity(leftPath, timeStep);
	origRightVelocity = velocity(rightPath, timeStep);

	//Funciona /
	//         ||

	//copy smooth velocities into fix velocities
	smoothCenterVelocity = origCenterVelocity;
	smoothLeftVelocity = origLeftVelocity;
	smoothRightVelocity = origRightVelocity;
	//Smooth velocity with zero final V
	smoothCenterVelocity[smoothCenterVelocity.size() - 1][1] = 0.0;
	smoothLeftVelocity[smoothLeftVelocity.size() - 1][1] = 0.0;
	smoothRightVelocity[smoothRightVelocity.size() - 1][1] = 0.0;

	smoothCenterVelocity = velocityFix(smoothCenterVelocity, origCenterVelocity, 0.0000001);
	smoothLeftVelocity = velocityFix(smoothLeftVelocity, origLeftVelocity, 0.0000001);
	smoothRightVelocity = velocityFix(smoothRightVelocity, origRightVelocity, 0.0000001);


	printPath(smoothPath);

	//cout << "FINISHED!!!!" << endl;
}





EctoPathOptimize::~EctoPathOptimize() {
	// TODO Auto-generated destructor stub
}
