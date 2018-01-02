#ifndef __PATHRUNNER_H_
#define __PATHRUNNER_H_

#include "Subsystems/DriveTrain/DriveTrain.h"
#include "MotionProfiles/MotionProfile.h"
#include "Path.h"

class PathRunner {
public:
	explicit PathRunner(Path &path);
	
	virtual Path &getPath() {
		return currentPath;
	}
	
	virtual void runPath(DriveTrain &driveTrain, double speed) = 0;

protected:
	Path &currentPath;
};

#endif