/*
* Settings
* Group: Motors
* motorQuantity: Amount of motors in the robot
* motor1 - motor#: id of motor (starting from 1)
* motorId.1 - motorId.#: optional motor name (starting from 1)
* motorInvert.1 - motorInvert.#: optional motor inversion (starting from 1, defaults to false)
* motorEncoderCodes.1 - motorEncoderCodes.#: optional encoder codes per rotation (starting from 1)
 */

#ifndef BOTBUSTERSREBIRTH_ECTOMOTORS_H
#define BOTBUSTERSREBIRTH_ECTOMOTORS_H

#include "Controllers/EctoTalon.h"
#include "Utilities/EctoSettings.h"
#include "Utilities/EctoUtils.h"
#include <vector>
#include <map>
#include <utility>
#include <string>
#include "spdlog/spdlog.h"

using namespace std;
namespace spd = spdlog;

struct motor {
	unsigned int id;
	std::string motorName;
	shared_ptr<EctoTalon> motor;
};

/**
 * Class used to control all Talons.
 * @return
 */

class EctoMotors final {
public:
	
	static EctoMotors &getInstance() {
		static EctoMotors singletonInstance;
		return singletonInstance;
	}
	
	
	shared_ptr<EctoTalon> getMotor(const string &name);
	
	void configPID(EctoTalon &motor, const EctoUtils::PidValues &pid, unsigned int profileSlot);
	
	void configPID(EctoTalon &motor, double p, double i, double d, double f, unsigned int profileSlot);
	
	void setupSlave(EctoTalon &slaveMotor, EctoTalon &masterMotor);

private:
	shared_ptr<spd::logger> log = spd::stdout_color_mt("EctoMotors");
	/** Vector with all motor IDs. */
	vector<unsigned int> ids;
	
	/** Used to store ID names. */
	map<unsigned int, string> names;
	
	/** Stores motor objects using motor IDs. */
	map<unsigned int, motor> motors;
	
	/** Stores IDs using names as keys. */
	map<string, unsigned int> idName;
	
	/** Stores motor inversions. */
	map<unsigned int, bool> inverted;
	
	/** Stores the amount of encoder codes per rotation per motor */
	map<unsigned int, uint16_t> encoderCodes;
	
	/** Stores whether or not the output will be inverted on closed loop modes */
	map<unsigned int, bool> closedLoopInversions;
	
	/** Stores encoder inversions. */
	map<unsigned int, bool> encoderInversions;
	
	EctoSettings &settings = EctoSettings::getInstance();
	
	EctoMotors();
	
	EctoMotors &operator=(const EctoMotors &);
	
	double motorQuantity = 0;
	std::size_t motorsMapSize = 0;
	
	const string baseStringId = "Motor";
	const string baseStringName = "MotorId.";
	const string baseStringInvert = "MotorInvert.";
	const string baseStringClosedLoopInvert = "MotorClosedLoopInvert.";
	const string baseStringEncoderCodes = "MotorEncoderCodes.";
	const string baseStringEncoderInvert = "MotorEncoderInvert.";
	
	const string invalidIdErrorMessage = "ATTEMPTED TO USE INVALID MOTOR ID: ";
	
	shared_ptr<EctoTalon> getMotor(unsigned int id);
	
	unsigned int getId(const string &input);
	
	inline bool checkIfValidId(unsigned int id);
	
	inline bool checkIfValidId(const string &name);
	
	inline void idCheck(unsigned int id);
	
	inline void idCheck(const string &name);
	
	void initializeMotors();
	
	void writeInversions();
	
	void writeClosedLoopInversions();
	
	void writeEncoderCodes();
	
	void writeEncoderInversions();
};

#endif
