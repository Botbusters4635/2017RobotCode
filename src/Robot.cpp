#include "Subsystems/EctoRobot.h"
#include "Subsystems/Mechanism/GearReceiver.h"
#include "MotionProfiles/MotionProfileExecuter.h"

/***********************************
EctoRebirth 2017 By:               *
Abiel Fernandez                    *
Alberto Jahuey                     *
Karen Rodriguez                    *
Carrillo Josephino                 *
 ***********************************/


using namespace std;
namespace spd = spdlog;

class Robot : public EctoRobot {
private:
	NavX &gyro = NavX::getInstance();
	shared_ptr<spd::logger> log = spd::get("log");
	GearReceiver &gearReceiver = GearReceiver::getInstance();
	TankState &state = TankState::getInstance();
	
public:
	void RobotInit() override {
		log->info("Robot Init");
		gyro.Reset();
		gyro.ZeroYaw();
		
		manager->initTelemetry();
	}
	
	void RobotPeriodic() override {
		Twist2d &current = *state.getLastState();
		SmartDashboard::PutNumber("x", current.getX());
		SmartDashboard::PutNumber("y", current.getY());
		SmartDashboard::PutNumber("angle", current.getTheta());
		
		manager->updateTelemetry();
	}
	
	void DisabledInit() override {
		log->info("Disabled Init");
		manager->initDisabled();
	}
	
	void DisabledPeriodic() override {
		manager->updateDisabled();
	}
	
	void AutonomousInit() override {
		log->info("Autonomous Init");
		
		gyro.ZeroYaw();
		settings.refreshFile();
		manager->initAuto();
	}
	
	void AutonomousPeriodic() override {
		manager->updateAuto();
	}
	
	void TeleopInit() override {
		log->info("Teleop Init");
		
		settings.refreshFile();
		manager->initTeleop();
		
	}
	
	void TeleopPeriodic() override {
		manager->updateTeleop();
	}
	
};

START_ROBOT_CLASS(Robot)
