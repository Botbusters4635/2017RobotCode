//
// Created by abiel on 11/25/17.
//

#include "Point.h"

double Point::getDistancePoints(const Point &point1, const Point &point2) {
	return sqrt(pow(point1.getX() - point2.getX(), 2) + pow(point1.getY() - point2.getY(), 2));
}

double Point::getAngleToPoint(const Point &initialPoint, const Point &targetPoint) {
	return EctoUtils::radToDeg(
			atan2(targetPoint.getY() - initialPoint.getY(), targetPoint.getX() - initialPoint.getX()));
}

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}

void Point::setX(double x) {
	this->x = x;
}

double Point::getX() const {
	return x;
}

void Point::setY(double y) {
	this->y = y;
}

double Point::getY() const {
	return y;
}
