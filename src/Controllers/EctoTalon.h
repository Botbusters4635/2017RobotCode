//
// Created by alberto on 13/12/17.
//

#ifndef BOTBUSTERSREBIRTH_ECTOTALON_H
#define BOTBUSTERSREBIRTH_ECTOTALON_H

#include <ctrlib/CANTalon.h>
#include "Utilities/EctoUtils.h"

class EctoTalon : public CANTalon {
public:
	explicit EctoTalon(int id);
	
	//TODO Hide get velocity
	
	double getSpeedRPM() const;
	
	double getSpeedMS(double wheelDiameter) const;
	
	void setBrake(bool brakeState);
	
};


#endif //BOTBUSTERSREBIRTH_ECTOTALON_H
