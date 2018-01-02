#ifndef BOTBUSTERSREBIRTH_TRANSLATION2D_H
#define BOTBUSTERSREBIRTH_TRANSLATION2D_H

#include <iostream>
#include <memory>
#include <cmath>

using namespace std;

/**
 * Represents a point in an X, Y plane.
 * @param x X Value
 * @param y Y Value
 */

class Translation2d {
public:
	Translation2d(double x, double y);
	
	Translation2d();
	
	explicit Translation2d(shared_ptr<Translation2d> other);
	
	Translation2d(shared_ptr<Translation2d> start, shared_ptr<Translation2d> end);
	
	double norm();
	
	double norm2();
	
	double x();
	
	double y();
	
	void setX(double x);
	
	void setY(double y);
	
	double x_, y_;
};

#endif
