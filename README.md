# 2017RobotCode

Overview
--------
This is the FRC robot code that Botbusters 4635 developed during the 2017 Off-season.  The main goal we set ourselves was that of
implementing an algorithm that would allow us to follow an easily modifiable, pre-defined path, we achieved this through implementing what is known as RobotState (an idea taken in part from Chezy Puffs 254 FRC Team), this tool allows us to know the relative coordinate the robot is currently at in relation to the position where it started. Using this data and a Pure Pursuit algorithm we are able to simply upload .path files to the roboRio, and have the program fetch waypoints from them, resulting in reliable path following.

Main code features
------------------

>- An "EctoSubsystem" manager that is in charge of calling every relevant function to operate any mechanism/system
>- Every "EctoSubsystem" automatically adds itself to the manager, and it also needs to define telemetry functions
>- A configuration file system in the form of EctoSettings, which allows us to change parameters without recompiling the code
>- Completly made by students
