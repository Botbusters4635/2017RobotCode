#include "EctoUtils.h"
#include "EctoSettings.h"

/**
 * Reads PIDs from settings, does not change group.
 * @param name
 * @return
 */
shared_ptr<EctoUtils::PidValues> EctoUtils::readPIDs(const string name) {
	EctoSettings &settings = EctoSettings::getInstance();
	
	auto output = make_shared<PidValues>();
	
	output->p = settings.getNumber(name + "_P");
	output->i = settings.getNumber(name + "_I");
	output->d = settings.getNumber(name + "_D");
	output->f = settings.getNumber(name + "_F");
	
	return output;
}

/**
 * Reads PIDs from settings, changes group.
 * @param name
 * @param group
 * @return
 */
shared_ptr<EctoUtils::PidValues> EctoUtils::readPIDs(const string name, const string group) {
	EctoSettings &settings = EctoSettings::getInstance();
	
	settings.setCurrentGroup(group);
	return readPIDs(name);
}


