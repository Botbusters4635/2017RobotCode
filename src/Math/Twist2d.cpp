#include "Twist2d.h"

/**
 * Represents a velocity vector with dx, dy and dtheta (rotation)
 * @param dx     X Value
 * @param dy     Y Value
 * @param dtheta Theta
 */
Twist2d::Twist2d(double dx, double dy, double dtheta) : Point(dx, dy) {
	this->dtheta = dtheta;
	
}

double Twist2d::getTheta() const {
	return dtheta;
}

Twist2d Twist2d::identity() {
	return *kIdentity;
}

shared_ptr<Twist2d> Twist2d::scaled(double scale) {
	return make_shared<Twist2d>(x * scale, y * scale, dtheta * scale);
}
