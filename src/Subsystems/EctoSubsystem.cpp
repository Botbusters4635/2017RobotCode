//
// Created by alberto on 13/12/17.
//
#include "EctoSubsystem.h"

EctoSubsystem::EctoSubsystem(const string &name) {
	manager.addSubsystem(this);
}

bool EctoSubsystem::isEnabledManaged() const {
	return enabledManaged;
}

void EctoSubsystem::enableManaged() {
	if (!enabledManaged) {
		enabledManaged = true;
	}
}

void EctoSubsystem::disableManaged() {
	if (enabledManaged) {
		enabledManaged = false;
	}
}

string EctoSubsystem::getName() const {
	return name;
}


///SUBSYTEM MANAGER DEFINITIONS
SubsystemManager::SubsystemManager() {
	managerEnabled = settings.getBool("ManagerEnabled");
	log->info("Initializing SubsystemManager, enabled: {}...", managerEnabled);
}

void SubsystemManager::addSubsystem(EctoSubsystem *newSubsystem) {
	log->trace("Adding {} to vector...", newSubsystem->getName());
	subsystems.emplace_back(newSubsystem);
}

void SubsystemManager::initDisabled() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->initDisabled();
		}
	}
}

void SubsystemManager::updateDisabled() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->updateDisabled();
		}
	}
}

void SubsystemManager::initTeleop() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->initTeleop();
		}
	}
	
}

void SubsystemManager::updateTeleop() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->updateTeleop();
		}
	}
}

void SubsystemManager::initAuto() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->initAuto();
		}
	}
}

void SubsystemManager::initTelemetry() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->sendInitialTelemetry();
		}
	}
}

void SubsystemManager::updateTelemetry() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->sendTelemetry();
		}
	}
}

void SubsystemManager::updateAuto() {
	if (managerEnabled) {
		for (const auto &subsystem : subsystems) {
			if (subsystem->isEnabledManaged())
				subsystem->updateAuto();
		}
	}
}



