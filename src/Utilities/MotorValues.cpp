#include "MotorValues.h"

/**
 * Initialize empty motor values
 */
MotorValues::MotorValues() = default;

/**
 * Initialize using mecanum values.
 * @param in Mecanum Values
 */
MotorValues::MotorValues(const mecanumValues in) {
	writeValue(in);
}

/**
 * Initialize using tank values.
 * @param in Tank Values
 */
MotorValues::MotorValues(const tankValues in) {
	writeValue(in);
}

/**
 * Writes values to buffer.
 * @param in mecanumValues struct
 */
void MotorValues::writeValue(const mecanumValues in) {
	if (driveTrain != DriveTrainSetting::Mecanum && driveTrain != DriveTrainSetting::Undefined)
		throw logic_error("Attempted to write using a different Drive Train!");
	else
		driveTrain = DriveTrainSetting::Mecanum;
	
	values.emplace_back(in.frontLeft);
	values.emplace_back(in.frontRight);
	values.emplace_back(in.backLeft);
	values.emplace_back(in.backRight);
}

/**
 * Writes values to buffer.
 * @param in tankValues struct
 */
void MotorValues::writeValue(const tankValues in) {
	if (driveTrain != DriveTrainSetting::Tank && driveTrain != DriveTrainSetting::Undefined)
		throw logic_error("Attempted to write using a different Drive Train!");
	else
		driveTrain = DriveTrainSetting::Tank;
	
	values.emplace_back(in.left);
	values.emplace_back(in.right);
}

/**
 * Returns shared pointer of mecanum values with stored values
 * @return
 */
shared_ptr<mecanumValues> MotorValues::readValueMecanum() {
	if (driveTrain != DriveTrainSetting::Mecanum)
		throw logic_error("Attempted to read using a different Drive Train!");
	
	auto output = make_shared<mecanumValues>();
	
	output->frontLeft = values[0];
	output->frontRight = values[1];
	output->backLeft = values[2];
	output->backRight = values[3];
	
	return output;
}

/**
 * Returns shared pointer of tank values with stored values
 * @return
 */
shared_ptr<tankValues> MotorValues::readValueTank() {
	if (driveTrain != DriveTrainSetting::Tank)
		throw logic_error("Attempted to read using a different Drive Train!");
	
	auto output = make_shared<tankValues>();
	
	output->left = values[0];
	output->right = values[1];
	
	return output;
}

/**
 * Calculates max value (Absolute).
 * @return
 */
double MotorValues::calculateMax() {
	if (driveTrain == DriveTrainSetting::Mecanum) {
		auto calc = readValueMecanum();
		return max(max(abs(calc->frontLeft), abs(calc->frontRight)), max(abs(calc->backLeft), abs(calc->backRight)));
	} else if (driveTrain == DriveTrainSetting::Tank) {
		auto calc = readValueTank();
		return max(abs(calc->left), abs(calc->right));
	} else {
		throw logic_error("Attempted to get max of empty value!");
	}
}

void MotorValues::limitValues(const double minIn, const double maxIn, const double minOut, const double maxOut) {
	if (driveTrain == DriveTrainSetting::Mecanum) {
		auto calc = readValueMecanum();
		
		calc->frontLeft = EctoUtils::map(calc->frontLeft, minIn, maxIn, minOut, maxOut);
		calc->frontRight = EctoUtils::map(calc->frontRight, minIn, maxIn, minOut, maxOut);
		calc->backLeft = EctoUtils::map(calc->backLeft, minIn, maxIn, minOut, maxOut);
		calc->backRight = EctoUtils::map(calc->backRight, minIn, maxIn, minOut, maxOut);
		
		writeValue(*calc);
	} else if (driveTrain == DriveTrainSetting::Tank) {
		auto calc = readValueTank();
		
		calc->left = EctoUtils::map(calc->left, minIn, maxIn, minOut, maxOut);
		calc->right = EctoUtils::map(calc->right, minIn, maxIn, minOut, maxOut);
		
		writeValue(*calc);
	} else {
		throw logic_error("Attempted to set limit of empty value!");
	}
}

void MotorValues::convertRadiansToRPM() {
	if (driveTrain == DriveTrainSetting::Mecanum) {
		auto calc = readValueMecanum();
		
		calc->frontLeft = EctoUtils::radToRPM(calc->frontLeft);
		calc->frontRight = EctoUtils::radToRPM(calc->frontRight);
		calc->backLeft = EctoUtils::radToRPM(calc->backLeft);
		calc->backRight = EctoUtils::radToRPM(calc->backRight);
		
		writeValue(*calc);
	} else if (driveTrain == DriveTrainSetting::Tank) {
		auto calc = readValueTank();
		
		calc->left = EctoUtils::radToRPM(calc->left);
		calc->right = EctoUtils::radToRPM(calc->right);
		
		writeValue(*calc);
	} else {
		throw logic_error("Attempted to use an empty value to convert to RPM!");
	}
}

/**
 * Returns set rotation value
 * @return
 */
const double MotorValues::getRotation() {
	return angle;
}

/**
 * Sets rotation
 * @param in Rotation Value
 */
void MotorValues::setRotation(const double in) {
	angle = in;
}

/**
 * Returns defined Drive Train
 * @return
 */
DriveTrainSetting MotorValues::getDriveTrain() {
	return driveTrain;
}

/**
 * Prints out motor values
 */
void MotorValues::printValues() {
	if (driveTrain == DriveTrainSetting::Mecanum) {
		auto temp = readValueMecanum();
		
		cout << "Front Left: " << temp->frontLeft << " " << "Front Right: " << temp->frontRight << endl;
		cout << "Back Left: " << temp->backLeft << " " << "Back Right: " << temp->backRight << endl;
		cout << endl;
	} else if (driveTrain == DriveTrainSetting::Tank) {
		auto temp = readValueTank();
		
		cout << "Left: " << temp->left << " " << "Right: " << temp->right << endl;
		cout << endl;
	} else {
		throw logic_error("Attempted to print undefined value!");
	}
}
