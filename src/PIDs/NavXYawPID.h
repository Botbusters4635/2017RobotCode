#ifndef BOTBUSTERSREBIRTH_NAVXYAWPID_H
#define BOTBUSTERSREBIRTH_NAVXYAWPID_H

#include "WPILib.h"
#include "Sensors/NavX.h"

class NavXYawPID : public PIDSource {
public:
	NavXYawPID();
	
	double PIDGet() override;
	
	void resetYaw();
	
	void zeroYaw();

private:
	NavX &gyro = NavX::getInstance();
};

#endif
