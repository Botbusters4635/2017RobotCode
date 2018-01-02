//
// Created by abiel on 11/26/17.
//

#ifndef BOTBUSTERSREBIRTH_MOTIONPROFILEPOINT_H
#define BOTBUSTERSREBIRTH_MOTIONPROFILEPOINT_H


class MotionProfilePoint {
public:
	MotionProfilePoint(double position, double velocity, double duration);
	
	double position, velocity, duration;
};

#endif //BOTBUSTERSREBIRTH_MOTIONPROFILEPOINT_H
