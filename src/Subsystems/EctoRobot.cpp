//
// Created by alber on 12/6/2017.
//

#include "EctoRobot.h"

EctoRobot::EctoRobot() {
	settings.setCurrentGroup("Main");
	spd::set_async_mode(8192);
	log = spd::stdout_color_mt("log");
	manager = &SubsystemManager::getInstance();
	
	int logLevel;
	logLevel = (int) settings.getNumber("consoleLogLevel");
	spd::level::level_enum levelRead;
	bool validLevel = false;
	
	if (logLevel >= 0 && logLevel <= 6) {
		validLevel = true;
		levelRead = spd::level::level_enum(logLevel);
	} else {
		levelRead = spd::level::trace;
		logLevel = 0;
	}
	
	spd::set_level(levelRead);
	if (!validLevel)
		log->critical("Couldn't read logLevel or invalid entry from settings.ini, defaults to trace (0)");
	log->info("Log level set to: {}", logLevel);
	
	log->info("Botbusters 4635 | 2018 Code");
	
	driveTrainSetting = settings.getString("DriveTrain");
	if (driveTrainSetting == "tank") {
		log->info("Detected Tank DriveTrain");
		settings.changeFile("/home/lvuser/settingsTank.ini");
		driveTrain = &EctoTank::getInstance();
	} else if (driveTrainSetting == "mecanum") {
		log->info("Detected Mecanum DriveTrain");
		settings.changeFile("/home/lvuser/settingsMecanum.ini");
		driveTrain = &EctoMecanum::getInstance();
	} else {
		log->warn("Could not detect DriveTrain, defaulting to Tank");
		settings.changeFile("/home/lvuser/settingsTank.ini");
		driveTrain = &EctoTank::getInstance();
	}
	
}
