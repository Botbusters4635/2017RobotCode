
#ifndef BOTBUSTERSREBIRTH_MECHANISM_H
#define BOTBUSTERSREBIRTH_MECHANISM_H

#include "Subsystems/Telemetry/EctoTelemetry.h"
#include "Utilities/EctoSettings.h"
#include <string>
#include <memory>
#include "spdlog/spdlog.h"
namespace spd = spdlog;

using namespace std;

class EctoSubsystem;

class SubsystemManager {
public:
	static SubsystemManager &getInstance() {
		static SubsystemManager singletonInstance;
		return singletonInstance;
	}
	
	void addSubsystem(EctoSubsystem *newSubsystem);
	
	void initDisabled();
	
	void updateDisabled();
	
	void initTeleop();
	
	void updateTeleop();
	
	void initAuto();
	
	void updateAuto();
	
	void initTelemetry();
	
	void updateTelemetry();


private:
	SubsystemManager();
	shared_ptr<spd::logger> log = spd::stdout_color_mt("SubsystemManager");
	
	EctoSettings &settings = EctoSettings::getInstance();
	bool managerEnabled = false;
	vector<EctoSubsystem *> subsystems;
};


class EctoSubsystem : public EctoTelemetry {
public:
	explicit EctoSubsystem(const string &name);
	
	virtual void initTeleop() = 0;
	
	virtual void updateTeleop() = 0;
	
	virtual void initAuto() = 0;
	
	virtual void updateAuto() = 0;
	
	virtual void initDisabled() = 0;
	
	virtual void updateDisabled() = 0;
	
	bool isEnabledManaged() const;
	
	void enableManaged();
	
	void disableManaged();
	
	string getName() const;

private:
	SubsystemManager &manager = SubsystemManager::getInstance();
	
	bool enabledManaged = true;
	
	string name;
};


#endif

