//
// Created by alber on 12/2/2017.
//

#include "PathParser.h"

PathParser::PathParser(string pathsDir) {
	this->pathsDir = std::move(pathsDir);
	
}

shared_ptr<Path> PathParser::getPath(string pathName) {
	vector<string> files = getPathsOnDir();
	string pathLocation;
	pathLocation.append(pathsDir);
	if (pathLocation.back() != '/')
		pathLocation.append("/");
	
	if(pathName.substr(pathName.length() - 5, pathName.length()) != ".path")
		pathName.append(".path");
	
	pathLocation.append(pathName);
	vector<shared_ptr<Waypoint>> waypoints;
	bool foundPath = false;
	for (const auto &i : files) {
		if (i == pathName) {
			foundPath = true;
			ifstream file(pathLocation);
			
			string stringBuffer;
			double numberBuffer;
			int lineCounter = 0;
			while (getline(file, stringBuffer)) {
				lineCounter++;
				stringstream streamBuffer(stringBuffer);
				vector<double> readings;
				int counter = 0;
				while (streamBuffer >> numberBuffer) {
					counter++;
					if (counter > 3)
						break;
					readings.emplace_back(numberBuffer);
					if (streamBuffer.peek() == ',') {
						streamBuffer.ignore();
					}
				}
				if (readings.size() == 3) {
					waypoints.emplace_back(make_shared<Waypoint>(readings[0], readings[1], readings[2]));
				} else if (readings.size() == 2) {
					waypoints.emplace_back(make_shared<Waypoint>(readings[0], readings[1], 0.0));
				} else {
					log->critical("Invalid file formatting on PathParser for file: {} on line: {} ",pathLocation, lineCounter);
					log->critical("Found: {} numbers, expected 2 or 3" , readings.size());
					assert(false);
				}
			}
			file.close();
			
			if (lineCounter == 0) {
				log->critical("Could not read any line from file: {} on PathParser", pathLocation);
				assert(false);
			}
			break;
		}
	}
	if (!foundPath) {
		log->critical("Could not find specified path!!! Attempted to find: {} ", pathLocation );
		assert(false);
	}
	shared_ptr<Path> path = make_shared<Path>(waypoints, pathName);
	return path;
}

vector<string> PathParser::getPathsOnDir() {
	DIR *dp;
	vector<string> files;
	struct dirent *dirp;
	if ((dp = opendir(pathsDir.c_str())) == nullptr) {
		log->critical("Error ({}) opening: {} see if the desired path is correct for PathParser", errno ,pathsDir );
		assert(false);
	}
	
	while ((dirp = readdir(dp)) != nullptr) {
		string fileName = string(dirp->d_name);
		if(fileName.substr(fileName.length() - 5, fileName.length()) == ".path"){
			files.emplace_back(string(dirp->d_name));
		}
	}
	closedir(dp);
	return files;
}
