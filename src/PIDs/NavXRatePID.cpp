#include "NavXRatePID.h"

NavXRatePID::NavXRatePID() = default;

double NavXRatePID::PIDGet() {
	return gyro.GetRate();
}
