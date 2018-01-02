/*
A set of various utilities, mostly used for robot code
 */
#ifndef BOTBUSTERSREBIRTH_ECTOUTILS_H
#define BOTBUSTERSREBIRTH_ECTOUTILS_H

#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include "Math/Point.h"

using namespace std;

namespace EctoUtils {

//Used to keep track of what DriveTrain is being used
	enum DriveTrainSetting {
		Undefined,
		Tank,
		Mecanum
	};
	
	struct MotorIdsMecanum {
		string frontLeft;
		string frontRight;
		string backLeft;
		string backRight;
	};
	
	struct MotorIdsTank {
		unsigned int left;
		unsigned int right;
	};
	
	struct PidValues {
		double p = 0;
		double i = 0;
		double d = 0;
		double f = 0;
	};
	
	constexpr double pidiv180 = M_PI / 180;
	constexpr double angledivpi = 180 / M_PI;

//TODO Check all the math

//Converts radians to rotation
	template<class T>
	
	inline T radToRot(T in) {
		return in / (M_PI * 2);
	}

//Converts rotation to radians
	template<class T>
	
	inline T rotToRad(T in) {
		return (in / 360.0) * (M_PI * 2.0);
	}

//Converts radians to RPM
	template<class T>
	
	inline T radToRPM(T in) {
		return radToRot<T>(in) * 60.0;
	}

//Converts radians to angle
	template<class T>
	
	inline T radToDeg(T in) {
		return in * angledivpi;
	}

//Converts angles to radians
	template<class T>
	
	inline T degToRad(T in) {
		return in * pidiv180;
	}

//Converts rotations to meters
	template<class T, class D>
	
	inline T rotationsToMeters(T rotations, D diameter) {
		return rotations * (diameter * M_PI);
	};

//Converts meters to rotations
	template<class T, class D>
	
	inline T metersToRotations(T meters, D diameter) {
		return meters / (diameter * M_PI);
	};

//Arduino map
	template<class T>
	
	T map(T x, T in_min, T in_max, T out_min, T out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

//Limits value from lower limit to upper limit
	template<class T>
	
	T limit(T value, T lowerLimit, T upperLimit) {
		if (lowerLimit > upperLimit)
			throw logic_error("Lower limit is larger than upper limit!");
		
		return max(lowerLimit, min(value, upperLimit));
	}

//Limits value from -1 to 1
	template<class T>
	
	T limit(T value) {
		return limit(value, -1.0, 1.0);
	}

//Implementation of deadband
	template<class T, class D>
	
	T applyDeadband(T value, D deadband) {
		if (abs(value) > deadband) {
			if (value > 0.0) {
				return (value - deadband) / (1.0 - deadband);
				
			} else {
				return (value + deadband) / (1.0 - deadband);
			}
		} else {
			return 0.0;
		}
	};

//Applies treshold to value
	template<class T, class D>
	
	T applyTreshold(T previousValue, T value, D treshold) {
		if (abs(abs(value) - abs(previousValue)) > treshold) {
			return value;
		} else {
			return previousValue;
		}
	};
	
	template<class T>
	
	T modRespectSign(T numer, T denom) {
		T sign = numer < 0.0 ? -1.0 : 1.0;
		
		T calc = fmod(abs(numer), denom);
		
		return calc * sign;
	}
	
	template<class T>
	
	T radianLoopBack(T in) {
		return asin(sin(in));
	}
	
	template<class T>
	
	T deegresLoopback(T number) {
	
	}
	
	template<class T, class D>

//Converts ms to seconds
	inline T msToSec(D in) {
		return in / 1000.0;
	};
	
	template<class T, class D>

//Converts seconds to ms
	inline T secToMs(D in) {
		return in * 1000.0;
	};
	
	template<class T>
	T shortestAngleTurn(T currentAngle, T target) {
		double angleToTurn = target - currentAngle;
		
		if (abs(angleToTurn) > 180) {
			if (currentAngle > 0) {
				if (angleToTurn > 0) {
					angleToTurn = angleToTurn - 180;
				} else {
					angleToTurn = angleToTurn + 360;
				}
			} else {
				if (angleToTurn > 0) {
					angleToTurn = angleToTurn - 360;
				} else {
					angleToTurn = angleToTurn + 180;
				}
			}
		}
		return angleToTurn;
	}

//Convert encoder codes per rev to Talon native units
	template<class T>
	inline T codesPerRevToNativeUnits(T codesPerRev) {
		return codesPerRev * 4;
	}

//Convert Talon native untis to encoder codes per rev
	template<class T>
	inline T nativeUnitsPerRev(T nativeUnits) {
		return nativeUnits / 4;
	}

//Converts RPM to NativeUnits as defined in p. 88 of Talon Software Manual
	template<class T>
	inline T RPMToNativeUnits(T rpm, T nativeUnitsPerRev) {
		return rpm * (1.0 / 60.0) * (1.0 / 10.0) * nativeUnitsPerRev;
	}

//Converts NativeUnits to RPM as defined in p. 88 of Talon Software Manual
	template<class T>
	inline T nativeUnitsToRPM(T nativeUnits, T nativeUnitsPerRev) {
		return nativeUnits / (1.0 / 60.0) / (1.0 / 10.0) / nativeUnitsPerRev;
	}

//Wheel diameter in meters
	template<class T>
	inline T RPMToMS(T rpm, T wheelDiameter) {
		return (rpm * wheelDiameter * M_PI) / 60;
	}
	
	template<class T>
	inline T MSToRPM(T ms, T wheelDiameter) {
		return (ms * 60) / (wheelDiameter * M_PI);
	}

//Reads PIDs from setting
	shared_ptr<EctoUtils::PidValues> readPIDs(string name);
	
	shared_ptr<EctoUtils::PidValues> readPIDs(string name, string group);
	
	template<typename T>
	T *ptr(T &obj) { return &obj; } //turn reference into pointer!
	
	template<typename T>
	T *ptr(T *obj) { return obj; } //obj is already pointer, return it!
	
}

#endif
