#include "TankState.h"

TankState::TankState() {
	settings.setCurrentGroup("EctoState");
	sum.angle = 0.0;
	sum.mag = 0.0;
	wheelDiameter = settings.getNumber("WheelDiameter");
	gyroTresh = settings.getNumber("GyroThreshold");
	threadUpdateSec = settings.getNumber("RobotStateThreadUpdate");
	stateUpdateSec = settings.getNumber("RobotStatePointUpdate");
	isEnabled = settings.getBool("Enabled");
	maxLoggedStates = (int) settings.getNumber("MaxLoggedStates");
	gyroPrevValue = 0;
	
	threadUpdate = thread(&TankState::update, this);
}


double TankState::getLeftDistance() {
	double distance = 0;
	distance = motors.getMotor("LeftMaster")->GetPosition();
	lDistance = distance;
	lDistance = lDistance * M_PI * wheelDiameter;
	return lDistance;
}

double TankState::getRightDistance() {
	double distance = 0;
	distance = motors.getMotor("RightMaster")->GetPosition();
	
	rDistance = distance;
	rDistance = rDistance * M_PI * wheelDiameter;
	
	return rDistance;
}

double TankState::getTime() {
	nextTime = Time::now();
	return std::chrono::duration_cast<std::chrono::duration<double>>(nextTime - startTime).count();
}

void TankState::addPoint() {
	double current = getTime();
	currentDistance = getDistanceTraveled();
	
	if (current - lastTime > stateUpdateSec) {
		double distanceDiff = currentDistance - lastDistance;
		vec    newPoint{};
		newPoint.mag   = distanceDiff;
		newPoint.angle = getYawNavX();
		
		double xOld = sum.mag * cos(sum.angle);
		double yOld = sum.mag * sin(sum.angle);
		
		double xNew    = newPoint.mag * cos(newPoint.angle);
		double yNew    = newPoint.mag * sin(newPoint.angle);
		double updateX = xNew + xOld;
		double updateY = yNew + yOld;
		
		shared_ptr<Twist2d> output = make_shared<Twist2d>(updateX, updateY, EctoUtils::radToDeg(newPoint.angle));
		
		if (values.size() < maxLoggedStates){
			values.emplace_back(output);
			times.emplace_back(current);
		}else{
			rotate(values.begin(), values.begin() + 1, values.end());
			values[values.size()-1].reset();
			values[values.size()-1] = output;
			
			rotate(times.begin(), times.begin() + 1, times.end());
			times[times.size()-1] = current;
		}
		sum.mag = sqrt(pow(updateX, 2) + pow(updateY, 2));
		
		if (updateX != 0)
			sum.angle = atan(updateY / updateX);
		else
			sum.angle = 0;
		lastDistance = currentDistance;
		
		lastTime = current;
	}
}

double TankState::getDistanceTraveled() {
	return (getLeftDistance() + getRightDistance()) / 2.0;
}

double TankState::getYawNavX() {
	double yaw = navx.GetYaw();
	
	double finalYaw = EctoUtils::applyTreshold(gyroPrevValue, yaw, gyroTresh);
	
	gyroPrevValue = finalYaw;
	finalYaw = EctoUtils::degToRad(finalYaw);
	return finalYaw;
}

void TankState::printLastState() {
	if (!values.empty()) {
		cout << "Count: " << values.size();
		cout << " X: " << values.back()->getX() << " Y: " << values.back()->getY() << " ROT: "
		     << values.back()->getTheta() << endl;
	}
}

shared_ptr<Twist2d> TankState::getLastState() {
	if (!values.empty())
		return values.back();
	else
		return make_shared<Twist2d>(0, 0, 0);
}

void TankState::start() {
	if (!isEnabled) {
		isEnabled = true;
		threadUpdate = thread(&TankState::update, this);
	}
	
}

void TankState::stop() {
	if (isEnabled) {
		isEnabled = false;
		threadUpdate.join();
	}
}


void TankState::update() {
	while (isEnabled) {
		addPoint();
		SmartDashboard::PutNumber("RightPosition", motors.getMotor("RightMaster")->GetPosition());
		SmartDashboard::PutNumber("LeftPosition", motors.getMotor("LeftMaster")->GetPosition());
		this_thread::sleep_for(std::chrono::duration<double>(threadUpdateSec));
	}
}
