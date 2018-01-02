#ifndef BOTBUSTERSREBIRTH_MOTORVALUES_H
#define BOTBUSTERSREBIRTH_MOTORVALUES_H

#include "Utilities/EctoUtils.h"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

using namespace std;
using EctoUtils::DriveTrainSetting;

struct mecanumValues {
	double frontLeft;
	double frontRight;
	double backLeft;
	double backRight;
};

struct tankValues {
	double left;
	double right;
};

class MotorValues {
public:
	MotorValues();
	
	explicit MotorValues(mecanumValues in);
	
	explicit MotorValues(tankValues in);
	
	void writeValue(mecanumValues in);
	
	void writeValue(tankValues in);
	
	shared_ptr<mecanumValues> readValueMecanum();
	
	shared_ptr<tankValues> readValueTank();
	
	double calculateMax();
	
	void limitValues(double minIn, double maxIn, double minOut, double maxOut);
	
	void convertRadiansToRPM();
	
	const double getRotation();
	
	void setRotation(double in);
	
	DriveTrainSetting getDriveTrain();
	
	void printValues();

private:
	DriveTrainSetting driveTrain = DriveTrainSetting::Undefined;
	double angle;
	vector<double> values;
};

#endif
