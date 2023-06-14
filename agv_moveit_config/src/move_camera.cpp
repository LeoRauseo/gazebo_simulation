#include "ros/ros.h"
#include "common_function.h"
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/Twist.h>
#include <agv_moveit_config/variable.h>
#include <std_msgs/Int32.h>


// The circle constant tau = 2*pi. One tau is one rotation in radians.
const double tau = 2 * M_PI;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "move_camera");
  ros::NodeHandle node_handle("~");



  ros::AsyncSpinner spinner(1);
  spinner.start();
  bool use_fsm = false;
  node_handle.getParam("use_fsm", use_fsm);
  int32_t desired_state=0;


  if(use_fsm) {
        if(getState( desired_state,node_handle))
            ROS_INFO_NAMED("trial", "State Machine Working");
  }



  static const std::string PLANNING_GROUP = "arm";
  moveit::planning_interface::MoveGroupInterface move_group_interface(PLANNING_GROUP);

  // class to add and remove collision objects in our "virtual world" scene
  // Raw pointers are frequently used to refer to the planning group for improved performance.
  const moveit::core::JointModelGroup* joint_model_group =
      move_group_interface.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

  // We can print the name of the reference frame for this robot.
  ROS_INFO_NAMED("trial", "Planning frame: %s", move_group_interface.getPlanningFrame().c_str());

  // We can also print the name of the end-effector link for this group.
  ROS_INFO_NAMED("trial", "End effector link: %s", move_group_interface.getEndEffectorLink().c_str());

  // We can get a list of all the groups in the robot:
  ROS_INFO_NAMED("tutorial", "Available Planning Groups:");
  std::copy(move_group_interface.getJointModelGroupNames().begin(),
            move_group_interface.getJointModelGroupNames().end(), std::ostream_iterator<std::string>(std::cout, ", "));
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  move_group_interface.setJointValueTarget(move_group_interface.getNamedTargetValues("look_camera"));

  bool success = (move_group_interface.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

  ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
  move_group_interface.move();


    if(use_fsm) {
        if(updateState(desired_state,node_handle))
            ROS_INFO_NAMED("trial", "State Machine Uptaded");
        else ROS_INFO_NAMED("trial", "State Machine ERROR state %d",desired_state);
    }


  ros::shutdown();
  return 0;


}
