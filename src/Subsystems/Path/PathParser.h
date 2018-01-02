//
// Created by alber on 12/2/2017.
//

#ifndef BOTBUSTERS_REBIRTH_PATHPARSER_H
#define BOTBUSTERS_REBIRTH_PATHPARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include "Path.h"
#include "Waypoint.h"
#include <sys/types.h>
#include <memory>
#include <dirent.h>
#include <cerrno>
#include "spdlog/spdlog.h"

using namespace std;
namespace spd = spdlog;

class PathParser {
public:
	explicit PathParser(string pathsDir);
	
	shared_ptr<Path> getPath(string pathName);
	vector<string> getPathsOnDir();

private:
	shared_ptr<spd::logger> log = spd::get("log");
	
	string pathsDir;
	
};


#endif //BOTBUSTERS_REBIRTH_PATHPARSER_H
