#ifndef BOTBUSTERSREBIRTH_ECTOSETTINGS_H
#define BOTBUSTERSREBIRTH_ECTOSETTINGS_H

#include "EctoUtils.h"
#include "inih/INIReader.h"
#include <mutex>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include "spdlog/spdlog.h"


using namespace std;
namespace spd = spdlog;

/*
    Singleton EctoSettings class
    Made by Alberto c:

    This class manages value readings from a file named settings.ini,
    So that we can easily modify parameters without the need to recompile.

    Usage:
    You need to declare a reference to this singleton, this is done as follows
        EctoSettings& settings = EctoSettings::getInstance();

    Then, you define which group you want to work with
        settings.setCurrentGroup("MOTORS");

    Afterwards, you can start retrieving values from that group
        double value =  settings.GetNumber("value");

    You can also use the functions as follows, describing the desired group
    and ignoring the "currentGroup"
        double value = settings.getNumber("values", "group");

    IN-CLASS Usage:
    The process is very similar to the one already described, the only difference being
    how the reference to the singleton is declared and defined.
        Header File (.h)
            Here we declare a member Reference to the class where it will be used.
            Remember to include EctoSetting's Header File (Example)
                class Test () {
                    Test();
                    EctoSettings& settings;
                };
        Source File (.cpp)
                Here we initialize the reference on the "Initialization List", notice this is done on the constructor
                    Test::Test() : settings(EctoSettings::getInstance()){

                    }

*/

class EctoSettings final {
public:
	constexpr static auto MAIN_SETTINGS_FILE_PATH = "/home/lvuser/settings.ini";
	
	void refreshFile();
	
	void setCurrentGroup(string newGroup);
	
	template<class T = double>
	T getNumber(const std::string &value, const std::string &group = currentGroup) {
		
		if (!initialGroupSet) {
			log->critical("Tried to use settings without an initial group!");
			assert(false);
		}
		
		std::unique_lock<std::mutex> lck(mtx);
		double valueNum = 0;
		if (fileLoaded) {
			valueNum = reader.GetReal(group, value, DEFAULT_NUMBER);
			if (valueNum == DEFAULT_NUMBER) {
				errorPromptNum(value);
			}
		}
		log->trace("File: {} Key: {} Group: {} Value: {}", currentFile, value, group,  valueNum);
		
		return valueNum;
	}
	
	string getString(const std::string &value, const std::string &group = currentGroup);
	
	bool getBool(const std::string &value, const std::string &group = currentGroup);
	
	template<class T = double>
	
	vector<T> getArrayNumbers(const std::string &value, const std::string &group = currentGroup) {
		if (!initialGroupSet) {
			log->critical("Tried to use settings without an initial group!");
			assert(false);
		}
		
		vector<T> vect;
		if (fileLoaded) {
			string input = getString(value, group);
			stringstream sStream(input);
			double buffer;
			while (sStream >> buffer) {
				vect.emplace_back(buffer);
				if (sStream.peek() == ',') {
					sStream.ignore();
				}
			}
		} else {

		}
		return vect;
	}
	
	vector<string> getArrayStrings(const std::string &value, const std::string &group = currentGroup);
	
	string getCurrentFile() const;
	
	void changeFile(string newFile);
	
	static EctoSettings &getInstance() {
		static EctoSettings singletonInstance;
		return singletonInstance;
	}

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("EctoSettings");
	
	const string SETTINGS_FILE_PATH = "/home/lvuser/settings.ini";
	const string errorAccessingFile = "Couldn't load file";
	const string errorChangingFile = "Couldn't change file, returning to previous valid .ini";
	const string errorReturnBool = "Couldn't get boolean, returning false.";
	const string errorReturnNum = "Couldn't get number, returning -1.";
	const string errorReturnStr = "Couldn't get string, returning ' '.";
	const string errorNotInitialGroup = "Group not set.";
	
	const double DEFAULT_NUMBER = -1;
	const string DEFAULT_STRING;
	const bool DEFAULT_BOOL = false;
	static string currentGroup;
	
	EctoSettings();
	
	EctoSettings(const EctoSettings &);
	
	EctoSettings &operator=(const EctoSettings &);
	
	INIReader reader;
	string currentFile;
	
	bool fileLoaded = false;
	bool initialGroupSet = false;
	
	void errorPromptNum(std::string value);
	
	void errorPromptStr(std::string value);
	
	void errorPromptBool(std::string value);
	
	std::mutex mtx;           // mutex for critical section
};

#endif
