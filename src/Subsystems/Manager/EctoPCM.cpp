#include "EctoPCM.h"

EctoPCM::EctoPCM() {
	// BOOST_LOG_TRIVIAL(trace) <<"EctoPCM initializing...";
	
	settings.setCurrentGroup("PCM");
	pcm = new PCM(static_cast<UINT8>(settings.getNumber("PcmId")));
	compressor = new Compressor(static_cast<int>(settings.getNumber("CompressorId")));
	
	compressor->ClearAllPCMStickyFaults();
	
	initializePistons();
}

/**
 * Reads piston IDs from settings.
 */
void EctoPCM::initializePistons() {
	// BOOST_LOG_TRIVIAL(trace) <<"Pistons initializing...";
	pistonQuantity = static_cast<unsigned int>(settings.getNumber("PistonQuantity"));
	
	for (unsigned int i = 1; i <= pistonQuantity; ++i) {
		vector<unsigned int> ids;
		pair<unsigned int, unsigned int> id;
		
		std::string name = settings.getString(basePistonName + to_string(i));
		
		ids = settings.getArrayNumbers<unsigned int>(basePiston + to_string(i));
		
		id.first = ids[0];
		id.second = ids[1];
		
		pistons.emplace(i, id);
		names.emplace(i, name);
		idNames.emplace(name, i);
	}
}

/**
 * Sets compressor power state.
 * @param state Compressor Power
 */
void EctoPCM::compressorPower(const bool state) {
	if (state) {
		compressorStatus = true;
		compressor->Start();
	} else {
		compressorStatus = false;
		compressor->Stop();
	}
}

/**
 * Sets piston using individual IDs.
 * @param id1   Solenoid ID
 * @param id2   Solenoid ID
 * @param state Piston State
 */
void EctoPCM::setPiston(const unsigned int id1, const unsigned int id2, const bool state) {
	//BOOST_LOG_TRIVIAL(trace) << "Updating pistons state, modifying valves " << id1 << " to " << state << " and " << id2 << " to " << !state ;
	pcm->SetSolenoid((unsigned char) id1, state);
	pcm->SetSolenoid((unsigned char) id2, !state);
}

/**
 * Sets piston state.
 * @param name  Piston Name
 * @param state Piston State
 */
void EctoPCM::setPiston(const string name, const bool state) {
	//BOOST_LOG_TRIVIAL(trace) <<"Updating piston " << name << " to " << !state ;
	unsigned int id = idNames[name];
	setPiston(pistons[id].first, pistons[id].second, state);
}

/**
 * Sets piston state.
 * @param id    Piston ID
 * @param state Piston State
 */
void EctoPCM::setPiston(const unsigned int id, const bool state) {
	// BOOST_LOG_TRIVIAL(warning) << "Please try using named pistons instead";
	//BOOST_LOG_TRIVIAL(trace) <<"Updating piston " << id << " to " << !state ;
	setPiston(pistons[id].first, pistons[id].second, state);
}

/**
 * Toggles piston.
 * @param name Piston Name
 */
void EctoPCM::togglePiston(const string name) {
	//BOOST_LOG_TRIVIAL(trace) <<"Toggling piston " << name;
	unsigned int id = idNames[name];
	invertState[id] = !invertState[id];
	setPiston(name, invertState[id]);
}

/**
 * Toggles piston.
 * @param id Piston ID
 */
void EctoPCM::togglePiston(const unsigned id) {
	// BOOST_LOG_TRIVIAL(warning) << "Please try using named pistons instead";
	// BOOST_LOG_TRIVIAL(warning) <<"Toggling piston " << id;
	invertState[id] = !invertState[id];
	setPiston(id, invertState[id]);
}

void EctoPCM::initPistonsState(const string name, bool state) {
	// BOOST_LOG_TRIVIAL(info) << "Initializing piston " << name << "...";
	unsigned int id = idNames[name];
	invertState[id] = state;
}
