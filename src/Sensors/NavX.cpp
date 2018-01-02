#include "NavX.h"

NavX::NavX() : AHRS(SerialPort::kMXP) {
	Reset();
	ResetDisplacement();
	ZeroYaw();
}
