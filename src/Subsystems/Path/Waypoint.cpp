//
// Created by Abiel on 11/9/17.
//
#include "Waypoint.h"

Waypoint::Waypoint(double x, double y, double time) : Point(x, y) {
	this->time = time;
}

double Waypoint::getTime() const {
	return time;
}



