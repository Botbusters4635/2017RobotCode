# Botbusters 2017 Robot Code

Overview
--------
This is our FRC Robot Code that we developed during 2017. Our main goal for this year was to implement an algorithim that would allow us to follow an easily modifiable pre-defined path. We achieved this by implementing what is known as RobotState (An idea taken in part from The Cheesy Poofs 254 FRC Team), this tool allows us to know the relative coordinate the robot is currently at. Using this data and a Pure Pursuit algorithm we are able to simply upload .path files to the roboRio, and have the program fetch waypoints from them, resulting in reliable path following.

Notable features
------------------

>- Modular, each "EctoSubsystem" can implement it's own telemetry and functions that are called periodically.
>- These modules are managed automatically by a manager.
>- Easily configurable, this allows us to change parameters without recompiling the code
>- EctoState, the position of the robot can be estimated at any time
