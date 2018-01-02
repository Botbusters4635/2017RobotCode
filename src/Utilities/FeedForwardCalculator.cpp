//
// Created by abiel on 11/29/17.
//

#include "FeedForwardCalculator.h"

FeedForwardCalculator::FeedForwardCalculator() {
	log->info("Initializing FeedForwardCalculator...");
	timer.reset(new Timer());
}

void FeedForwardCalculator::setMotor(string motorName, uint16_t codesPerRev) {
	log->info("Setting motor: {} with codesPerVev: {}...", motorName, codesPerRev);
	this->motorName = motorName;
	
	this->codesPerRev = codesPerRev;
	
	//Resets values
	maxSpeed = 0.0;
	fGain = 0.0;
	
	finished = false;
	running = false;
	
	log->trace("Getting previous Talon Control Mode...");
	previousControlMode = motor->GetTalonControlMode();
	
	log->trace("Resetting timer...");
	timer->Reset();
}

/*
 * Sets the talon to run at max speed
 */
void FeedForwardCalculator::setMaxSpeed() {
	log->trace("Setting Talon to max speed: {}...", maxTalonSpeed );
	motor->SetTalonControlMode(CANTalon::TalonControlMode::kThrottleMode);
	
	motor->Set(maxTalonSpeed);
}

/*
 * Gets the speed at which the talon is running
 */
double FeedForwardCalculator::getMaxSpeed() {
	double rpm = motor->getSpeedRPM();
	log->trace("Getting max speed from Talon, got: {}", rpm);
	return rpm;
	//return EctoUtils::RPMToNativeUnits(rpm, (double) codesPerRev);
}

/*
 * Calculates the F-gain based on the talon's max speed
 */
double FeedForwardCalculator::calculateFgain() {
	double fGain = (1023) / EctoUtils::RPMToNativeUnits(maxSpeed, (double) codesPerRev * 4);
	log->trace("Calculating F-gain, got: {}", fGain);
	return fGain;
}

/*
 * Resets talon to previous state
 */
void FeedForwardCalculator::resetTalon() {
	log->trace("Resetting Talon to previous mode...");
	motor->Set(0);
	motor->SetTalonControlMode(previousControlMode);
}

/*
 * Needs to be called periodically in order to calculate f-gain
 */
bool FeedForwardCalculator::periodic() {
	if(!started){
		log->info("Started calculating F-Gain");
		started = true;
	}
	if (!finished) {
		
		if (!running) {
			timer->Start();
			setMaxSpeed();
			running = true;
		} else if (timer->HasPeriodPassed(timeToWaitToAccelerate)) {
			log->info("Finished");
			maxSpeed = getMaxSpeed();
			resetTalon();
			
			fGain = calculateFgain();
			
			finished = true;
		}
		
	}
	
	return finished;
}

double FeedForwardCalculator::getMaxSpeedRPM() {
	log->trace("Getting max rpm from Talon, got: {}", maxSpeed);
	return maxSpeed;
	//return EctoUtils::nativeUnitsToRPM(maxSpeed, (double) codesPerRev);
}

double FeedForwardCalculator::getMaxNativeSpeed() {
	double maxNativeSpeed = EctoUtils::RPMToNativeUnits(maxSpeed, (double) codesPerRev * 4);
	log->trace("Getting max native speed from Talon, got: {}", maxNativeSpeed);

	return maxNativeSpeed;
}

double FeedForwardCalculator::getFgain() {
	return fGain;
}

void FeedForwardCalculator::printValues() {
	log->info("Max RPM: {}", getMaxSpeedRPM());
	log->info("Max Native Speed: {}", getMaxNativeSpeed());
	log->info("Fgain: {}", getFgain());
}