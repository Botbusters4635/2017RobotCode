#ifndef BOTBUSTERSREBIRTH_ECTOKINEMATICS_H
#define BOTBUSTERSREBIRTH_ECTOKINEMATICS_H

#include "Math/Twist2d.h"
#include "Utilities/MotorValues.h"
#include <memory>

using namespace std;

/**
 * Abstract class for kinematics.
 * @return
 */
class EctoKinematics {
public:
	/**
	 * Calculates inverse kinematics, returns values from -1 to 1, unitless.
	 * @param  vX       X Velocity, preferably from -1 to 1
	 * @param  vY       Y Velocity, preferably from -1 to 1
	 * @param  rotation Rotation speed in radians
	 * @return
	 */
	virtual shared_ptr<MotorValues> inverseKinematics(double vX, double vY, double rotation) = 0;
	
	/**
	 * Calculates inverse kinematics, returns values in necessary wheel speeds in unit/unit (t).
	 * @param  vX       X Velocity, in unit/unit(t)
	 * @param  vY       Y Velocity, in unit/unit(t)
	 * @param  rotation Rotation speed in radians
	 * @param  radius   Radius of wheel in previously used units
	 * @return
	 */
	virtual shared_ptr<MotorValues> inverseKinematics(double vX, double vY, double rotation, double radius) = 0;
	
	/**
	 * Calculates inverse kinematics, unitless.
	 * @param  in Twist2d object with velocities
	 * @return
	 */
	virtual shared_ptr<MotorValues> inverseKinematics(const Twist2d &in) = 0;
	
	/**
	 * Calculates inverse kinematics, returns values of necessary wheel speeds in unit/unit (t).
	 * @param  in     Twist2d object with velocities
	 * @param  radius Radius of wheel in previously used units
	 * @return
	 */
	virtual shared_ptr<MotorValues> inverseKinematics(const Twist2d &in, double radius) = 0;
	
	/**
	 * Returns velocities without units.
	 * @param  in Motor Velocities
	 * @return
	 */
	virtual const shared_ptr<Twist2d> kinematics(shared_ptr<MotorValues> in) = 0;
	
	/**
	 * Returns velocities
	 * @param  in     Motor Velocities
	 * @param  radius Wheel Radius
	 * @return
	 */
	virtual const shared_ptr<Twist2d> kinematics(shared_ptr<MotorValues> in, double radius) = 0;
	
};

#endif
