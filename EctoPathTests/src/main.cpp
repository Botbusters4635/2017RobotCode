//============================================================================
// Name        : Pathtest.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <ctime>
#include "EctoPath.h"
#include "EctoPathOptimize.h"
#include "omp.h"
using namespace std;

EctoPath *pathtest;
EctoPathOptimize *pathtestoptimize;


void printSi(string in){
								cout << in << endl;
}

int main() {
								path waypoints{
																{1, 1},
																{5, 1},
																{9, 12},
																{12, 9},
																{15, 6},
																{19, 12}
								};

								const double totalTime = 1000;
								const double timeStep = 0.1;
								const double robotTrackWidth = 2;

								std::chrono::time_point<std::chrono::system_clock> start, end;
								start = std::chrono::system_clock::now();

								pathtest = new EctoPath(waypoints);
								pathtest->calculate(totalTime, timeStep, robotTrackWidth);

								end = std::chrono::system_clock::now();

								std::chrono::duration<double> elapsed_seconds = end-start;
								std::time_t end_time = std::chrono::system_clock::to_time_t(end);

								auto unoptimized = elapsed_seconds.count();

								start = std::chrono::system_clock::now();

								pathtestoptimize = new EctoPathOptimize(waypoints);
								pathtestoptimize->calculate(totalTime, timeStep, robotTrackWidth);
								end = std::chrono::system_clock::now();

								elapsed_seconds = end-start;
								end_time = std::chrono::system_clock::to_time_t(end);

								auto optimized = elapsed_seconds.count();

								std::cout << "Unoptimized: " << unoptimized << " Optimized: " << optimized << std::endl;
								std::cout << "Difference :" << unoptimized - optimized  << std::endl;

								//cout << "Same?: " << (pathtest.smoothPath) == (pathtestoptimize.smoothPath) << endl;

								//waypoints = pathtest->smoothRightVelocity;

								//waypoints = pathtest->NodeOnlyWayPoints(waypoints);

								//waypoints = pathtest->Inject(waypoints, 10);

								//waypoints = pathtest->smoother(waypoints, 0.7, 0.3, 0.0000001);

								//path testPath3 = pathtest->origPath;

								//pathtest->printPath(waypoints);

								//for(unsigned int i = 0; i < testPath3.size(); i++){
								//	string test = pathtest->ParseToString(testPath3[i]);
								//	cout << test << endl;
								//}



								return 0;
}
