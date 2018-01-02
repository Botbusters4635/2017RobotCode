/*
* Settings
* Group: PCM
* PcmId: PCM ID
* CompressorId: Compressor ID
* PistonQuantity: Piston Quantity
* piston1 - piston#: Piston IDs
* pistonId.1 - pistonId.#: Optional Piston Names
 */
#ifndef BOTBUSTERSREBIRTH_ECTOPCM_H
#define BOTBUSTERSREBIRTH_ECTOPCM_H

#include "WPILib.h"
#include "ctrlib/CANTalon.h"
#include <ctre/PCM.h>
#include <Compressor.h>
#include "Utilities/EctoSettings.h"
#include <map>
#include <utility>
#include <string>

using namespace std;

/**
 * Class used to control pistons.
 * @return
 */
class EctoPCM final {
public:
	
	static EctoPCM &getInstance() {
		static EctoPCM singletonInstance;
		return singletonInstance;
	}
	
	void compressorPower(bool state);
	
	void setPiston(unsigned int id1, unsigned int id2, bool state);
	
	void setPiston(string name, bool state);
	
	void setPiston(unsigned int id, bool state);
	
	void togglePiston(string name);
	
	void togglePiston(unsigned id);
	
	void initPistonsState(string name, bool state);

private:
	EctoPCM();
	
	EctoPCM &operator=(const EctoPCM &);
	
	EctoSettings &settings = EctoSettings::getInstance();
	
	void initializePistons();
	
	PCM *pcm;
	Compressor *compressor;
	bool compressorStatus = false;
	
	unsigned int pistonQuantity;
	
	const string basePiston = "piston";
	const string basePistonName = "pistonId.";
	
	map<unsigned int, pair<unsigned int, unsigned int>> pistons;
	map<unsigned int, std::string> names;
	map<std::string, unsigned int> idNames;
	map<unsigned int, bool> invertState;
};

#endif
