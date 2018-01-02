#ifndef BOTBUSTERSREBIRTH_TWIST2D_H
#define BOTBUSTERSREBIRTH_TWIST2D_H

#include <iostream>
#include <memory>
#include "Point.h"

using namespace std;

/**
 * Represents a velocity vector.
 * @param dx     X Value
 * @param dy     Y Value
 * @param dtheta Rotation
 */

class Twist2d : public Point {
public:
	Twist2d(double dx, double dy, double dtheta);
	
	double getTheta() const;
	
	Twist2d identity();
	
	shared_ptr<Twist2d> scaled(double scale);

protected:
	double dtheta;
	
	
	shared_ptr<Twist2d> kIdentity;
};

#endif
