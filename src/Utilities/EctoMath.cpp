//
// Created by Abiel on 10/30/17.
//

#include "EctoMath.h"

using EctoMath::Coord;

//TODO Find out what all of these functions do, and comment them

Coord EctoMath::splineCoords(const Spline &s, double percentage) {
	percentage = EctoUtils::limit(percentage, 0.0, 1.0); //Limit percentage to 0 - 1
	
	double x = percentage * s.knot_distance;
	double y = (s.a * x + s.b) *
	           pow(x, 4) + (s.c * x + s.d) *
	                       pow(x, 2) + s.e * x; //Magic thing that does magical things
	
	double cos_theta = cos(s.angle_offset);
	double sin_theta = sin(s.angle_offset);
	
	Coord c{
			x * cos_theta - y * sin_theta + s.x_offset,
			x * sin_theta + y * cos_theta + s.y_offset
	}; // Even more magical math things
	
	return c;
}

double EctoMath::splineDeriv2(const Spline &s, double knot_distance, double p) {
	double x = p * knot_distance;
	
	return (5 * s.a * x + 4 * s.b) + pow(x, 3) + (3 * s.c * x + 2 * s.d) * x + s.e;
}

double EctoMath::splineDistance(Spline &s, double sample_count) {
	double arc_length = 0, t = 0, dydt = 0;
	
	double deriv0 = splineDeriv2(s, s.knot_distance, 0);
	
	double integrand = 0;
	double last_integrand = sqrt(1 + pow(deriv0, 2)) / sample_count;
	
	for (unsigned int i = 0; i <= (unsigned int) sample_count; i++) {
		t = i / sample_count;
		
		dydt = splineDeriv2(s, s.knot_distance, t);
		
		integrand = sqrt(1 + pow(dydt, 2)) / sample_count;
		
		arc_length += (integrand + last_integrand) / 2;
		last_integrand = integrand;
	}
	
	arc_length = s.knot_distance * arc_length;
	s.arc_length = arc_length;
	return arc_length;
}

double EctoMath::progressSplineDistance(const Spline &s, double distance, double sample_count) {
	double arc_length = 0, t = 0, dydt = 0, last_arc_length = 0;
	
	double deriv0 = splineDeriv2(s, s.knot_distance, 0);
	
	double integrand = 0;
	double last_integrand = sqrt(1 + pow(deriv0, 2)) / sample_count;
	
	distance /= s.knot_distance;
	
	for (unsigned int i = 0; i <= (unsigned int) sample_count; i++) {
		t = i / sample_count;
		
		dydt = splineDeriv2(s, s.knot_distance, t);
		
		integrand = sqrt(1 + pow(dydt, 2)) / sample_count;
		
		arc_length += (integrand + last_integrand) / 2;
		
		if (arc_length > distance)
			break;
		
		last_integrand = integrand;
		last_arc_length = arc_length;
	}
	
	double interpolated = t;
	
	if (arc_length != last_arc_length) {
		interpolated += ((distance - last_arc_length)
		                 / (arc_length - last_arc_length) - 1) / sample_count;
	}
	
	return interpolated;
}

