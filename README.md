# gazebo_simulation
Gazebo simulation with HC10dt MOTOMAN + custom camera and supports  using MoveIt and AprilTag
# important CREDITS 
motoman packege is forked from https://github.com/ros-industrial/motoman and aprilta_ros package is forked from https://github.com/AprilRobotics/apriltag_ros.
The only difference is that apriltag_ros/src/common_function.cpp the camera frame id is set manually. 


REQUIRED MoveIt framework

# Goal 
The objective is to position a robot arm in front of the chuck of the CNC machine using MoveIt. 
I created the URDF file with the intention of replicating a real robot utilized in my laboratory.

# How to run 
See agv_moveit_config/launch/simulation.launch for a better understanding. 
To run the simulation

```bash
$ roslaunch agv_moveit_config simulation.launch use_fsm:=true
```
