#ifndef BOTBUSTERSREBIRTH_SIMPLEPID_H
#define BOTBUSTERSREBIRTH_SIMPLEPID_H

#include "WPILib.h"

/**
 * Creates a simple PIDOutput and PIDSource, which stores the value and sets it
 * with setValue or gets it with getValue respectively.
 */
class SimplePIDSource : public PIDSource {
public:
	SimplePIDSource();
	
	double PIDGet() override;
	
	void setValue(double input);

private:
	double value;
};

class SimplePIDOutput : public PIDOutput {
public:
	SimplePIDOutput();
	
	void PIDWrite(double input) override;
	
	double getValue();

private:
	double value;
};

#endif
