/*
* Settings
* Group: Climber
* OnSpeed: Speed used when climber is on (Usually 1)
* OffSpeed: Speed used when climber is off (Usually 0)
* Motors: Array of motors to use, the first one is assigned as a master, and the others are slaves
*/

#ifndef BOTBUSTERSREBIRTH_ECTOCLIMBER_H
#define BOTBUSTERSREBIRTH_ECTOCLIMBER_H

#include "Utilities/EctoSettings.h"
#include "Subsystems/Manager/EctoInput.h"
#include "Subsystems/Manager/EctoMotors.h"
#include "Subsystems/EctoSubsystem.h"
#include <vector>
#include <string>

using namespace std;

/**
 * Class which handles a climber, can be used with multiple motors.
 * @return
 */
class EctoClimber : public EctoSubsystem {
public:
	static EctoClimber &getInstance() {
		static EctoClimber singletonInstance;
		return singletonInstance;
	}
	
	double getClimberState() const;
	
	void initTeleop() override;
	
	void updateTeleop() override;
	
	void initAuto() override;
	
	void updateAuto() override;
	
	void initDisabled() override;
	
	void updateDisabled() override;
	void sendInitialTelemetry() const override;
	
	void sendTelemetry() const override;
	

private:
	EctoClimber();
	
	EctoTalon &climbMotor;
	
	EctoMotors &motors = EctoMotors::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	
	double onSpeed = 1;
	double offSpeed = 0;
	bool enabled = false;
	
	unsigned int masterId;
	
	vector<double> climberIds;
	
	vector<unsigned int> slaves;
};

#endif
