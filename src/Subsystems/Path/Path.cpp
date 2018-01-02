//
// Created by Abiel on 11/9/17.
//

#include "Path.h"

Path::Path(vector<shared_ptr<Waypoint>> &waypoints, const string &name) {
	addWaypoint(waypoints);
}

void Path::addWaypoint(shared_ptr<Waypoint> point) {
	waypoints.emplace_back(point);
}

void Path::addWaypoint(const vector<shared_ptr<Waypoint>> &waypoints) {
	for (const auto &point : waypoints) {
		addWaypoint(point);
	}
}

size_t Path::trajectorySize() {
	return waypoints.size();
}

double Path::getTrajectoryDistance() {
	double distance = 0;
	for (int i = 0; i < trajectorySize() - 1; ++i) {
		distance += Point::getDistancePoints(*waypoints[i], *waypoints[i + 1]);
	}
	return distance;
}

shared_ptr<Waypoint> Path::getWaypoint(unsigned int index) {
	assert(index <= waypoints.size());
	return waypoints[index];
}

string& Path::getName() {
	return name;
}
