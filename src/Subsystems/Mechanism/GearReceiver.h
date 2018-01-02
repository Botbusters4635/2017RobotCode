#ifndef BOTBUSTERSREBIRTH_GEARRECEIVER_H
#define BOTBUSTERSREBIRTH_GEARRECEIVER_H

#include "Subsystems/Manager/EctoPCM.h"
#include "Subsystems/Manager/EctoInput.h"
#include "Subsystems/EctoSubsystem.h"

class GearReceiver : public EctoSubsystem {
public:
	static GearReceiver &getInstance() {
		static GearReceiver singletonInstance;
		return singletonInstance;
	}
	
	void open();
	
	void set(bool target);
	
	void close();
	
	void toggle();
	
	bool getState() const;
	
	void initTeleop() override;
	
	void updateTeleop() override;
	
	void initAuto() override;
	
	void updateAuto() override;
	
	void initDisabled() override;
	
	void updateDisabled() override;
	
	void sendInitialTelemetry() const override;
	
	void sendTelemetry() const override;


private:
	EctoPCM &pcm = EctoPCM::getInstance();
	EctoInput &stick = EctoInput::getInstance();
	bool currentState;
	
	explicit GearReceiver();
	
	GearReceiver &operator=(const GearReceiver &);
};

#endif