#include "EctoSettings.h"

string EctoSettings::currentGroup;
using namespace std;

EctoSettings::EctoSettings() : reader(MAIN_SETTINGS_FILE_PATH) {
	log->trace("Initializing EctoSettings...");
	if (reader.ParseError() < 0) {
		log->error("Could not find file!");
	} else {
		fileLoaded = true;
		currentFile = MAIN_SETTINGS_FILE_PATH;
	}
	
}

void EctoSettings::errorPromptNum(std::string value) {
	log->warn("{} Attempted to get= {} from {}", errorReturnNum, value, currentFile);
	
}

void EctoSettings::errorPromptStr(std::string value) {
	log->warn("{} Attempted to get= {} from {}", errorReturnStr, value, currentFile);
	
}

void EctoSettings::errorPromptBool(std::string value) {
	log->warn("{} Attempted to get= {} from {}", errorReturnBool, value, currentFile);
	
}

void EctoSettings::refreshFile() {
	log->info("Refreshing settings file");
	changeFile(currentFile);
}

void EctoSettings::setCurrentGroup(const std::string newGroup) {
	log->trace("Settings current group set to= {}", newGroup);
	currentGroup = newGroup;
	initialGroupSet = true;
}

string EctoSettings::getCurrentFile() const {
	return currentFile;
}

void EctoSettings::changeFile(const std::string newFile) {
	std::unique_lock<std::mutex> lck(mtx);
	reader = INIReader(newFile);
	log->info("Settings file changed to= {}", newFile);
	if (reader.ParseError() < 0) {
		reader = INIReader(currentFile);
		log->critical("Could not find settings file= {}", newFile);
		assert(false);
	} else {
		log->info("Successfully changed settings file to= {}", newFile);
		currentFile = newFile;
	}
}

string EctoSettings::getString(const std::string &value, const std::string &group) {
	if (!initialGroupSet) {
		log->critical("Tried to use settings without an initial group!");
		assert(false);
	}
	
	std::unique_lock<std::mutex> lck(mtx);
	
	string valueStr;
	if (fileLoaded) {
		valueStr = reader.Get(group, value, DEFAULT_STRING);
		if (valueStr == DEFAULT_STRING) {
			errorPromptStr(value);
		}
	}
	log->trace("File: {} Key: {} Group: {} Value: {}", currentFile, value, group, valueStr);
	
	return valueStr;
}

bool EctoSettings::getBool(const std::string &value, const std::string &group) {
	if (!initialGroupSet) {
		log->critical("Tried to use settings without an initial group!");
		assert(false);
	}
	
	std::unique_lock<std::mutex> lck(mtx);
	
	bool valueBool;
	if (fileLoaded) {
		valueBool = reader.GetBoolean(group, value, DEFAULT_BOOL);
	} else {
		errorPromptBool(value);
		valueBool = DEFAULT_BOOL;
	}
	log->trace("File: {} Key: {} Group: {} Value: {}", currentFile, value, group, valueBool);
	
	return valueBool;
}

vector<string> EctoSettings::getArrayStrings(const std::string &value, const std::string &group) {
	if (!initialGroupSet) {
		log->critical("Tried to use settings without an initial group!");
		assert(false);
	}
	
	vector<string> vect;
	string input = getString(value, group);
	stringstream sStream(input);
	string buffer;
	while (std::getline(sStream, buffer, ',')) {
		vect.emplace_back(buffer);
	}
	return vect;
}
