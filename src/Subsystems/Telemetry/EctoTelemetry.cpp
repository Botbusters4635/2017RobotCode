//
// Created by Abiel on 12/12/17.
//

#include "EctoTelemetry.h"


void EctoTelemetry::enableTelemetry() const {
	enabled = true;
}

void EctoTelemetry::disableTelemetry() const {
	enabled = false;
}

bool EctoTelemetry::getTelemetryStatus() const {
	return enabled;
}
