#ifndef BOTBUSTERSREBIRTH_MECANUMROTATIONRATE_H
#define BOTBUSTERSREBIRTH_MECANUMROTATIONRATE_H

#include "Kinematics/EctoKinematicsMecanum.h"
#include "Subsystems/DriveTrain/EctoMecanum.h"
#include "WPILib.h"

//class EctoMecanum2;

class MecanumRotationRate : public PIDOutput {
public:
	MecanumRotationRate();
	
	void updateSpeeds();
	
	void PIDWrite(double output) override;

private:
	EctoMecanum &mecanum = EctoMecanum::getInstance();
	EctoKinematicsMecanum &kinematics = EctoKinematicsMecanum::getInstance();
	double x, y;
};

#endif
