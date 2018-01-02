#include "NavXYawPID.h"

NavXYawPID::NavXYawPID() = default;

/**
 * Returns the gyro yaw.
 * @return
 */
double NavXYawPID::PIDGet() {
	return gyro.GetYaw();
}

/**
 * Resets the yaw gyro, reduces drift.
 */
void NavXYawPID::resetYaw() {
	gyro.Reset();
}

/**
 * Set yaw to zero.
 */
void NavXYawPID::zeroYaw() {
	gyro.ZeroYaw();
}
