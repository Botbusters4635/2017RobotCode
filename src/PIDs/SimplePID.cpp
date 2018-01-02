#include "SimplePID.h"

SimplePIDOutput::SimplePIDOutput() {
	value = 0;
}

void SimplePIDOutput::PIDWrite(double input) {
	value = input;
}

double SimplePIDOutput::getValue() {
	return value;
}

SimplePIDSource::SimplePIDSource() {
	value = 0;
}

void SimplePIDSource::setValue(double input) {
	value = input;
}

double SimplePIDSource::PIDGet() {
	return value;
}
