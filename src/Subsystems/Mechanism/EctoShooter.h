/*
* Settings
* Group: Shooter
* Output: Normal Speed
* MaxOutput: Maximum Speed
* OutputRevolver: Revolver Speed
* WheelDiameterMeters: Diameter of Wheel
*/

//DEPRECIATED CLASS!!!

#include "Controllers/EctoTalon.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "Subsystems/Manager/EctoInput.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/EctoUtils.h"
#include "PIDs/SimplePID.h"
#include <WPILib.h>
#include <iostream>

using namespace std;

/**
 * Controls the shooter using PID
 */
class EctoShooter {
public:
	void update();
	
	void shootAuto();
	
	static EctoShooter &getInstance() {
		static EctoShooter singletonInstance;
		return singletonInstance;
	}

private:
	void shoot(double speed, int direction);
	
	EctoShooter();
	
	EctoShooter(const EctoShooter &);
	
	EctoShooter &operator=(const EctoShooter &);
	
	SimplePIDSource speed;
	PIDController speedPID;
	SimplePIDOutput outputPID;
	
	double maxOutput;
	
	double output, outputRevolver, wheelDiameter;
	EctoMotors &motors = EctoMotors::getInstance();
	
	EctoTalon &motor1, &motor2, &motorRevolver;
	EctoInput &stick = EctoInput::getInstance();
	EctoSettings &settings = EctoSettings::getInstance();
	
	Timer clock;
	
};
