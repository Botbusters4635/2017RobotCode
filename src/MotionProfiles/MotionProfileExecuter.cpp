//
// Created by abiel on 11/26/17.
//

#include "MotionProfileExecuter.h"

MotionProfileExecuter::MotionProfileExecuter(const shared_ptr<MotionProfile> &in, const string &motorName,
                                             const unsigned int profileSlot, const bool velocityOnly) : motor(
		*motors.getMotor(motorName)) {
	this->profile = in;
	this->motorName = motorName;
	this->velocityOnly = velocityOnly;
	this->profileSlot = profileSlot;
	
	//Generate profile if not generated
	if (!profile->getGeneratedStatus())
		profile->generateProfile();
	
	periodicNotifier.reset(new Notifier(&MotionProfileExecuter::periodicTask, this));
	
	setupTalon();
}

void MotionProfileExecuter::clearMotionProfile() {
	motor.ClearMotionProfileTrajectories();
	
	motor.ClearMotionProfileHasUnderrun();
}

CANTalon::TrajectoryPoint
MotionProfileExecuter::convertToTrajectoryPoint(const shared_ptr<MotionProfilePoint> in, const bool firstPoint,
                                                const bool lastPoint) {
	if (firstPoint && lastPoint) {
		throw logic_error("Invalid point!");
	}
	
	CANTalon::TrajectoryPoint temp{};
	
	temp.position = in->position;
	//temp.velocity = in->velocity / 4;
	temp.velocity = EctoUtils::MSToRPM(in->velocity, .15);
	
	//TODO Check if this gives the proper value
	temp.timeDurMs = EctoUtils::secToMs<unsigned int>(in->duration);
	
	temp.profileSlotSelect = this->profileSlot;
	
	temp.zeroPos = firstPoint;
	temp.isLastPoint = lastPoint;
	
	temp.velocityOnly = this->velocityOnly;
	
	return temp;
}

void MotionProfileExecuter::reset() {
	clearMotionProfile();
	
	motor.SetControlMode(previousControlMode);
}

void MotionProfileExecuter::setupTalon() {
	previousControlMode = motor.GetControlMode();
	
	//Sets control frame period to half of timestep
	motor.ChangeMotionControlFramePeriod(EctoUtils::secToMs<int>(profile->getTimeStep()) / 2);
	
	motor.SetControlMode(CANTalon::kMotionProfile);
	
	motor.Set(CANTalon::SetValueMotionProfileDisable);
}

void MotionProfileExecuter::sendToTalon() {
	CANTalon::TrajectoryPoint point{};
	
	bool firstPoint = false;
	bool lastPoint = false;
	
	motor.GetMotionProfileStatus(talonStatus);
	
	for (unsigned int i = 0; i < profile->getTotalPoints(); ++i) {
		//If i = 0, set firstPoint to true, if i is the last point, set lastPoint to true if neither condition is met, both are false
		if (i == 0) {
			firstPoint = true;
		} else if (i + 1 == profile->getTotalPoints()) {
			firstPoint = false;
			lastPoint = true;
		} else {
			firstPoint = false;
			lastPoint = false;
		}
		
		point = convertToTrajectoryPoint(profile->getData()[i], firstPoint, lastPoint);
		
		//log->info("Index: {} Velocity: {} FirstPoint: {} LastPoint: {}", i, point.velocity, point.zeroPos, point.isLastPoint);
		
		//Try to push point until PushMotionProfileTrajectory returns a nonzero value
		//while(!motor->PushMotionProfileTrajectory(point)) {
		//	motor->ProcessMotionProfileBuffer();
		//	motor->PushMotionProfileTrajectory(point);
		//}
		if ((firstPoint || lastPoint) || point.velocity != 0) {
			while (!motor.PushMotionProfileTrajectory(point)) {
				motor.ProcessMotionProfileBuffer();
			}
		}
	}
}

void MotionProfileExecuter::setStart(bool in) {
	start = in;
}

/**
 * Call Periodically
 */
void MotionProfileExecuter::periodicTask() {
	motor.ProcessMotionProfileBuffer();
}

void MotionProfileExecuter::controlTalon() {
	motor.GetMotionProfileStatus(talonStatus);
	
	motor.ProcessMotionProfileBuffer();
	
	motor.ClearMotionProfileHasUnderrun();
	
	switch (status) {
		case executerStatus::notFilled:
			clearMotionProfile();
			
			status = executerStatus::filling;
			
			break;
		
		case executerStatus::filling:
			if (firstFilling) {
				if (talonStatus.btmBufferCnt < minimumPoints) {
					motor.GetMotionProfileStatus(talonStatus);
					
					motor.ProcessMotionProfileBuffer();
				}
				
				//Stats periodic task
				periodicNotifier->StartPeriodic(this->profile->getTimeStep() / 2);
				
				firstFilling = false;
			}
			
			if (start) {
				motor.Set(CANTalon::SetValueMotionProfileEnable);
				status = executerStatus::running;
				sendToTalon();
			}
			
			break;
		
		case executerStatus::running:
			if (talonStatus.activePoint.isLastPoint) {
				status = executerStatus::finished;
				periodicNotifier->Stop();
			}
			
			motor.ClearMotionProfileHasUnderrun();
			break;
		
		case executerStatus::finished:
			motor.Set(CANTalon::SetValueMotionProfileDisable);
			break;
	}
	
}