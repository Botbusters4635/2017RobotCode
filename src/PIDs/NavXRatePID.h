#ifndef BOTBUSTERSREBIRTH_NAVXRATEPID_H
#define BOTBUSTERSREBIRTH_NAVXRATEPID_H

#include "WPILib.h"
#include "Sensors/NavX.h"

class NavXRatePID : public PIDSource {
public:
	NavXRatePID();
	
	double PIDGet() override;

private:
	NavX &gyro = NavX::getInstance();
};

#endif
