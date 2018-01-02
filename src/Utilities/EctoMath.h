//
// Created by Abiel on 10/30/17.
//

#ifndef BOTBUSTERSREBIRTH_ECTOMATH_H
#define BOTBUSTERSREBIRTH_ECTOMATH_H

#include "EctoUtils.h"
#include <cmath>

using namespace std;

namespace EctoMath {
	
	struct Coord {
		double x, y;
	};
	
	struct Spline {
		double a, b, c, d, e;
		double x_offset, y_offset, angle_offset, knot_distance, arc_length;
	};
	
	Coord splineCoords(const Spline &s, double percentage);
	
	double splineDeriv2(const Spline &s, double knot_distance, double p);
	
	double splineDistance(Spline &s, double sample_count);
	
	double progressSplineDistance(const Spline &s, double distance, double sample_count);
	
}

#endif