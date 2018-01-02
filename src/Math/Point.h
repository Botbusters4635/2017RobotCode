//
// Created by abiel on 11/25/17.
//

#ifndef BOTBUSTERSREBIRTH_POINT_H
#define BOTBUSTERSREBIRTH_POINT_H

#include <cmath>
#include <Utilities/EctoUtils.h>

class Point {
public:
	static double getDistancePoints(const Point &point1, const Point &point2);
	
	static double getAngleToPoint(const Point &initialWaypoint, const Point &targetWaypoint);
	
	Point(double x, double y);
	
	double getX() const;
	
	void setX(double x);
	
	double getY() const;
	
	void setY(double y);

protected:
	double x, y;
};

#endif //BOTBUSTERSREBIRTH_POINT_H
