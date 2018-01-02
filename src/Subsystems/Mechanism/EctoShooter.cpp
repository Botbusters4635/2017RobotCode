#include "EctoShooter.h"

//DEPRECIATED CLASS!!!

EctoShooter::EctoShooter() :
		speedPID(.03, 0.0054, 0, &speed, &outputPID),
		motor1(*motors.getMotor("Ball1")),
		motor2(*motors.getMotor("Ball2")),
		motorRevolver(*motors.getMotor("BallRevolver")) {
	
	settings.setCurrentGroup("Shooter");
	output = settings.getNumber("Output");
	maxOutput = settings.getNumber("MaxOutput");
	
	outputRevolver = settings.getNumber("OutputRevolver");
	wheelDiameter = settings.getNumber("WheelDiameterMeters");
	
	//motors.setFeedbackDevice(motor2Id, CANTalon::CtreMagEncoder_Relative);
	
	speedPID.SetSetpoint(output);
	speedPID.Enable();
	
	clock.Reset();
	clock.Start();
}

void EctoShooter::shoot(double speed, int direction) {
	
	/*
	int directionTime = 1;
	int directionTime2 = 3;
	int timeToWait = 6;


	//bool timePassed2 = clock.Get() > 8;

	if(timePassed){
			directionTime = directionTime * -1;

	}
	else{
			bool timePassed = clock.Get() > timeToWait;
	}

	if(timePassed){
			directionTime = 1;
	}
	else if(timePassed2){
			directionTime = -1;
			clock.Reset();
	}
	*/
	
	/*
	direction = 1;
	int timeToWait = 4;

	int delta, deltaDif;
	bool timePassed = clock.Get() > timeToWait + delta;
	bool state;

	cout << "Time: " << clock.Get() << endl;
	cout << "Delta: " << delta << endl;
	cout << "Time Passed: " << timePassed << endl;
	cout << "State: " << state << endl;

	if(timePassed){
			if(state){
					delta = 2 + clock.Get();
			}
			else{
					delta = 6 + clock.Get();
			}
			direction = direction * -1;
			state = !state;
	}
	*/
	
	
	
	cout << direction << endl;
	
	//cout << "Direction: " << direction << endl;
	motor1.Set(speed);
	motor2.Set(-speed);
	
	if (speed != 0)
		motorRevolver.Set(outputRevolver * direction);
	else
		motorRevolver.Set(0);
}

void EctoShooter::update() {
	bool xButton = stick.getButtonState(5);
	bool otherButton = stick.getButtonState(6);
	double leftTrigger = stick.getJoystickExpo("rightTrig", .2, 0);
	
	int revolverDirection = otherButton ? -1 : 1;
	//int revolverDirection = 1;
	
	//cout << leftTrigger << endl;
	//cout << "Joystick: " <<  EctoUtils::map(leftTrigger, 0, 1, output, maxOutput) << endl;
	speedPID.SetSetpoint(EctoUtils::map(leftTrigger, 0.0, 1.0, output, maxOutput));
	
	double speedMotor = (motor2.getSpeedMS(wheelDiameter));
	speed.setValue(speedMotor);
	
	//cout << "Shooter: " << speedMotor << endl;
	double outputPercent = outputPID.getValue();
	//cout << speedMotor << endl;
	
	SmartDashboard::PutNumber("SPEED SHOOTER", speedMotor);
	
	if (xButton) {
		shoot(outputPercent, revolverDirection);
	} else {
		shoot(0, revolverDirection);
	}
}

void EctoShooter::shootAuto() {
	speedPID.SetSetpoint(output);
	
	double speedMotor = motor2.getSpeedMS(wheelDiameter);
	
	speed.setValue(speedMotor);
	cout << "Shooter: " << speedMotor << endl;
	double outputPercent = outputPID.getValue();
	//cout << speedMotor << endl;
	SmartDashboard::PutNumber("SPEED SHOOTER", speedMotor);
	shoot(outputPercent, 1);
}
