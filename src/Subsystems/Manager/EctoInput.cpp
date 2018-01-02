#include "EctoInput.h"

EctoInput::EctoInput() {
	settings.setCurrentGroup("Input");
	DriverStation &driver = DriverStation::GetInstance();
	
	this->stick = new Joystick(static_cast<int>(settings.getNumber("StickPort")));
	if (this->stick->GetAxisCount() <= 0) {
		return;
	} else {
		joystickDetected = true;
	}
	
	deadzone = settings.getNumber("Deadzone");
	
	buttonCount = stick->GetButtonCount();
	axisCount = stick->GetAxisCount();
	
	initializeButtons();
	initializeAxis();
}

/**
 * Initializes all buttons and joysticks
 */
void EctoInput::initializeButtons() {
	for (int i = 1; i <= buttonCount; ++i) {
		pair<int, JoystickButton *> button;
		pair<int, string> name;
		pair<string, int> nameId;
		
		string buttonName = settings.getString(baseNameButton + to_string(i));
		
		name.first = i;
		name.second = buttonName;
		
		button.first = i;
		button.second = new JoystickButton(this->stick, i);
		
		buttons.emplace(button);
		buttonNames.emplace(name);
		
		if (buttonName != "\0") {
			nameId.first = buttonName;
			nameId.second = i;
			
			buttonIds.emplace(nameId);
		}
	}
}

void EctoInput::initializeAxis() {
	for (int i = 1; i <= axisCount; i++) {
		pair<int, string> name;
		pair<string, int> id;
		
		string buttonName = settings.getString(baseNameJoystick + to_string(i));
		
		name.first = i;
		name.second = buttonName;
		
		id.first = buttonName;
		id.second = i;
		
		joystickNames.emplace(name);
		joystickIds.emplace(id);
	}
}

/**
 * Check if given button ID is valid.
 * @param id 
 */
bool EctoInput::checkIfValidButton(const unsigned int id) {
	return id <= getButtonCount();
}

/**
 * Checks if button name exists.
 * @param name 
 */
bool EctoInput::checkIfValidButton(const string name) {
	return buttonIds.count(name) > 0;
}

/**
 * Checks if button ID is valid, if it is not, throws an exception.
 * @param id 
 */
void EctoInput::buttonCheck(const unsigned int id) {
	if (!checkIfValidButton(id)) {
		throw runtime_error(invalidButtonIdErrorMessage + to_string(id));
	}
}

/**
 * Checks if button name is valid, if it is not, throws an exception.
 * @param name 
 */
void EctoInput::buttonCheck(const string name) {
	if (!checkIfValidButton(name)) {
		throw runtime_error(invalidButtonIdErrorMessage + name);
	}
}

/**
 * Check if given axis ID is valid.
 * @param id 
 */
bool EctoInput::checkIfValidAxis(const unsigned int id) {
	return id <= getAxisCount();
}

/**
 * Checks if axis name exists.
 * @param name 
 * @return 
 */
bool EctoInput::checkIfValidAxis(const string name) {
	return joystickIds.count(name) > 0;
}

/**
 * Checks if axis id is valid, if it is not, throws an exception.
 * @param id 
 */
void EctoInput::axisCheck(const unsigned int id) {
	if (!checkIfValidAxis(id)) {
		throw runtime_error(invalidAxisIdErrorMessage + to_string(id));
	}
}

void EctoInput::axisCheck(const string name) {
	if (!checkIfValidAxis(name)) {
		throw runtime_error(invalidAxisIdErrorMessage + name);
	}
}

/**
 * Returns button state
 * @param  id Button ID
 * @return
 */
bool EctoInput::getButtonState(const int id) {
	buttonCheck(id);
	if (!joystickDetected) {
		throw runtime_error(joystickErrorMsg);
		return 0;
	}
	return stick->GetRawButton(id);
}

/**
 * Returns button state
 * @param  name Button name
 * @return
 */
bool EctoInput::getButtonState(const string name) {
	int id = buttonIds[name];
	return getButtonState(id);
}

/**
 * Sets a command to a button when pressed
 * @param buttonId Button ID
 * @param command  Command to set to button
 */
void EctoInput::setCommandButtonPressed(const int buttonId, frc::Command *command) {
	buttonCheck(buttonId);
	buttons[buttonId]->WhenPressed(command);
}

/**
 * Sets a command to a button when pressed
 * @param name    Button Name
 * @param command Command to set to button
 */
void EctoInput::setCommandButtonPressed(const string name, frc::Command *command) {
	int id = buttonIds[name];
	//R << "BUTTON" << id << endl;
	setCommandButtonPressed(id, command);
}

/**
 * Sets a command to a button while held
 * @param buttonId Button ID
 * @param command  Command to set to button
 */
void EctoInput::setCommandButtonHeld(const int buttonId, frc::Command *command) {
	//std::cout << "Test command held 2" << '\n';
	buttonCheck(buttonId);
	buttons[buttonId]->WhileHeld(command);
}

/**
 * Sets a command to a button while held
 * @param name    Button Name
 * @param command Command to set to button
 */
void EctoInput::setCommandButtonHeld(const string name, frc::Command *command) {
	int id = buttonIds[name];
	//cout << "ID " << id << '\n';
	//cout << "BUTTON" << id << endl;
	setCommandButtonHeld(id, command);
	//std::cout << "Test command held 1" << '\n';
}

/**
 * Returns joystick raw value
 * @param  name Axis Name
 * @return
 */
double EctoInput::getJoystickRaw(const string name) {
	axisCheck(name);
	int id = joystickIds[name];
	return getJoystickRaw(id);
}

/**
 * Returns joystick raw value
 * @param  joystickId Axis ID
 * @return
 */
double EctoInput::getJoystickRaw(const unsigned int joystickId) {
	axisCheck(joystickId);
	if (!joystickDetected) {
		throw runtime_error(joystickErrorMsg);
		return 0;
	}
	return stick->GetRawAxis(joystickId - 1);
}

/**
 * Returns joystick with deadzone applied
 * @param  joystickId Axis Name
 * @return
 */
double EctoInput::getJoystick(const int joystickId) {
	double value = getJoystickRaw(joystickId);
	
	return abs(value) > deadzone ? value : 0;
}

/**
 * Returns joystick with deadzone applied
 * @param  name Axis Name
 * @return
 */
double EctoInput::getJoystick(const string name) {
	//cout << name << joystickIds[name] << endl;
	axisCheck(name);
	return getJoystick(joystickIds[name]);
}

/**
 * Gets the joystick value, and applies an exponential curve
 * @param  joystickId      Joystick ID
 * @param  gainAdjustment  Exponential Curve Gain Adjustment
 * @param  inverseDeadband Exponential Curve Inverse Deadband Adjustment
 * @return
 */
double EctoInput::getJoystickExpo(const int joystickId, const double gainAdjustment, const double inverseDeadband) {
	double value = getJoystick(joystickId);
	
	double output;
	
	if (value >= 0) {
		output =
				inverseDeadband +
				(1 - inverseDeadband) * (gainAdjustment * pow(value, 3.0) + (1 - gainAdjustment) * value);
	} else {
		output =
				-inverseDeadband +
				(1 - inverseDeadband) * (gainAdjustment * pow(value, 3.0) + (1 - gainAdjustment) * value);
	}
	
	return output;
}

double EctoInput::getJoystickExpo(const string name, const double gainAdjustment, const double inverseDeadband) {
	//cout << name << " " << joystickIds[name] << " " << getJoystickExpo(joystickIds[name], gainAdjustment, inverseDeadband)
	//     << " " << gainAdjustment << endl;
	
	return getJoystickExpo(joystickIds[name], gainAdjustment, inverseDeadband);
}

int EctoInput::getButtonCount() {
	return buttonCount;
}

/**
 * Gets the axis count
 * @return
 */
int EctoInput::getAxisCount() {
	return axisCount;
}

int EctoInput::getButtonName(string in) {
	buttonCheck(in);
	return buttonIds[in] + 1;
}
