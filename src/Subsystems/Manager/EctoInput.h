/*
* Settings
* Group: Input
* StickPort: Stick Port (probably 0)
* Deadzone: Deadzone for Joystick
* ButtonName# - ButtonName#: Button Name
* AxisName# - AxisName#: Axis Name
*/

#ifndef BOTBUSTERSREBIRTH_ECTOINPUT_H
#define BOTBUSTERSREBIRTH_ECTOINPUT_H

#include "Utilities/EctoSettings.h"
#include "WPILib.h"
#include "Joystick.h"
#include <map>
#include <utility>
#include <string>

using namespace std;

/**
 * Class repsponsible for handling all inputs.
 * @return
 */
class EctoInput final {
public:
	static EctoInput &getInstance() {
		static EctoInput singletonInstance;
		return singletonInstance;
	}
	
	bool getButtonState(int id);
	
	bool getButtonState(string name);
	
	void setCommandButtonPressed(int buttonId, frc::Command *command);
	
	void setCommandButtonPressed(string name, frc::Command *command);
	
	void setCommandButtonHeld(int buttonId, frc::Command *command);
	
	void setCommandButtonHeld(string name, frc::Command *command);
	
	double getJoystickRaw(const unsigned int joystickId);
	
	double getJoystickRaw(string name);
	
	double getJoystick(string name);
	
	double getJoystick(int joystickId);
	
	double getJoystickExpo(int joystickId, double gainAdjustment, double inverseDeadband);
	
	double getJoystickExpo(string name, double gainAdjustment, double inverseDeadband);
	
	int getButtonCount();
	
	int getAxisCount();
	
	int getButtonName(string in);

private:
	EctoInput();
	
	EctoInput &operator=(const EctoInput &);
	
	EctoSettings &settings = EctoSettings::getInstance();
	
	void initializeButtons();
	
	void initializeAxis();
	
	Joystick *stick;
	
	const string invalidButtonIdErrorMessage = "ATTEMPTED TO USE INVALID BUTTON ID: ";
	const string invalidAxisIdErrorMessage = "ATTEMPTED TO USE INVALID AXIS ID: ";
	
	bool checkIfValidButton(unsigned int id);
	
	bool checkIfValidButton(string name);
	
	void buttonCheck(const unsigned int id);
	
	void buttonCheck(string name);
	
	bool checkIfValidAxis(unsigned int id);
	
	bool checkIfValidAxis(string name);
	
	void axisCheck(unsigned int id);
	
	void axisCheck(string name);
	
	const string baseNameJoystick = "AxisName";
	const string baseNameButton = "ButtonName";
	
	int buttonCount, axisCount;
	
	double deadzone;
	bool joystickDetected = false;
	const string joystickErrorMsg = "NO JOYSTICK DETECTED.\n";
	
	map<int, JoystickButton *> buttons;
	
	map<int, string> buttonNames;
	map<string, int> buttonIds;
	
	map<int, string> joystickNames;
	map<string, int> joystickIds;
};

#endif
