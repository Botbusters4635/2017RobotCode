#include "GearReceiver.h"

GearReceiver::GearReceiver() : EctoSubsystem("Gear Receiver") {
	currentState = false;
	close();
}

void GearReceiver::open() {
	currentState = true;
	pcm.setPiston("Gear", currentState);
}

void GearReceiver::set(bool target) {
	if (target) {
		open();
	} else {
		close();
	}
}

void GearReceiver::close() {
	currentState = false;
	pcm.setPiston("Gear", currentState);
}

void GearReceiver::toggle() {
	currentState = !currentState;
	pcm.setPiston("Gear", currentState);
}

bool GearReceiver::getState() const {
	return currentState;
}

void GearReceiver::initTeleop() {

}

void GearReceiver::updateTeleop() {
	set(stick.getButtonState("B"));
}

void GearReceiver::initAuto() {
	close();
}

void GearReceiver::updateAuto() {

}

void GearReceiver::initDisabled() {

}

void GearReceiver::updateDisabled() {

}

void GearReceiver::sendInitialTelemetry() const {
	SmartDashboard::PutBoolean("Gear Intaked Enabled", getState());
}

void GearReceiver::sendTelemetry() const {
	SmartDashboard::PutBoolean("Gear Intaked Enabled", getState());
}




