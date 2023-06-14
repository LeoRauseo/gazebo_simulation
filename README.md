# gazebo_simulation
Gazebo simulation with HC10dt MOTOMAN + custom camera and supports  using MoveIt and AprilTag
# important CREDITS 
motoman packege is forked from https://github.com/ros-industrial/motoman and aprilta_ros package is forked from https://github.com/AprilRobotics/apriltag_ros.
only difference is that aprilta_ros/src/common_function.cpp the camera frame id is set manually. 


REQUIRED MoveIt framework

# Goal 
The AGV, specifically its external dimensions, the HC10dt, and an RGB camera were utilized to create a simulated robot. 
The objective is to position the robot arm in front of the chuck of the CNC machine using MoveIt. 

# How to run 
See agv_moveit_config/launch/simulation.launch for a better understanding. 
To run the simulation

```bash
$ roslaunch agv_moveit_config simulation.launch use_fsm:=true
```
