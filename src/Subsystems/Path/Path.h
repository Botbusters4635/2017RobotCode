//
// Created by Abiel on 11/9/17.
//

#ifndef BOTBUSTERSREBIRTH_TRAJECTORY_H
#define BOTBUSTERSREBIRTH_TRAJECTORY_H

#include "Waypoint.h"
#include "Utilities/EctoUtils.h"
#include <cassert>
#include <vector>
#include <memory>

using namespace std;

class Path {
public:
	explicit Path(vector<shared_ptr<Waypoint>> &waypoints, const string &name);
	
	void addWaypoint(shared_ptr<Waypoint> point);
	
	void addWaypoint(const vector<shared_ptr<Waypoint>> &waypoints);
	
	size_t trajectorySize();
	
	double getTrajectoryDistance();
	
	shared_ptr<Waypoint> getWaypoint(unsigned int index);
	
	string& getName();
protected:
	string name;
	double trajectoryDistance;
	vector<shared_ptr<Waypoint>> waypoints;
};

#endif //BOTBUSTERSREBIRTH_TRAJECTORY_H
