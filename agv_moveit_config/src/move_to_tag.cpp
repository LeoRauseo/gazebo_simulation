#include "ros/ros.h"
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/Twist.h>
#include <tf2/LinearMath/Quaternion.h>

#include <tf2_ros/transform_listener.h>
#include <agv_moveit_config/variable.h>

#include "common_function.h"

// The circle constant tau = 2*pi. One tau is one rotation in radians.
const double tau = 2 * M_PI;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "move_group_interface_tutorial");
  ros::NodeHandle node_handle("~");
  ros::AsyncSpinner spinner(1);
  spinner.start();
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);

  bool use_fsm = false;
  node_handle.getParam("use_fsm", use_fsm);

  int32_t desired_state=3;

  if(use_fsm) {
      if(getState( desired_state,node_handle))
          ROS_INFO_NAMED("execution", "State Machine Working");
  }


  static const std::string PLANNING_GROUP = "arm";
  moveit::planning_interface::MoveGroupInterface move_group_interface(PLANNING_GROUP);

    // class to add and remove collision objects in our "virtual world" scene
    // Raw pointers are frequently used to refer to the planning group for improved performance.

  const moveit::core::JointModelGroup *joint_model_group =
            move_group_interface.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

    // We can print the name of the reference frame for this robot.
  ROS_INFO_NAMED("execution", "Planning frame: %s", move_group_interface.getPlanningFrame().c_str());

    // We can also print the name of the end-effector link for this group.
  ROS_INFO_NAMED("execution", "End effector link: %s", move_group_interface.getEndEffectorLink().c_str());

    // We can get a list of all the groups in the robot:
  ROS_INFO_NAMED("execution", "Available Planning Groups:");
  std::copy(move_group_interface.getJointModelGroupNames().begin(),
              move_group_interface.getJointModelGroupNames().end(),
              std::ostream_iterator<std::string>(std::cout, ", "));
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;
  geometry_msgs::TransformStamped transformStamped;







     try {
            transformStamped = tfBuffer.lookupTransform("part_1", "pic_tag",
                                                        ros::Time(0));
     }
     catch (tf2::TransformException &ex) {
            ROS_WARN("%s", ex.what());
            ros::Duration(1.0).sleep();
            return 0;
     }

      geometry_msgs::Pose target_pose;

      target_pose.position.x=transformStamped.transform.translation.x;
      target_pose.position.y=transformStamped.transform.translation.y;
      target_pose.position.z=transformStamped.transform.translation.z;
      target_pose.orientation.x=transformStamped.transform.rotation.x;
      target_pose.orientation.y=transformStamped.transform.rotation.y;
      target_pose.orientation.z=transformStamped.transform.rotation.z;
      target_pose.orientation.w=transformStamped.transform.rotation.w;

      std::cout << target_pose.position.x << "   " << target_pose.position.y << "   " << target_pose.position.z << std::endl;

      move_group_interface.setPoseTarget(target_pose,"camera_link_view");
      move_group_interface.setGoalOrientationTolerance(0.00001);
      move_group_interface.setGoalTolerance(0.001);

      bool success=false;
      int count=0;

      do {
          count++;
          success = (move_group_interface.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
          ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
          move_group_interface.move();
          if (count==6) {

              success=true;
              ROS_INFO_NAMED("execution", "tried 5 times but failed");
              continue;
          }

      }
      while(!success);



      if(use_fsm) {
        if(updateState(desired_state,node_handle))
            ROS_INFO_NAMED("execution", "State Machine Uptaded");
        else ROS_INFO_NAMED("execution", "State Machine ERROR state %d",desired_state);
    }

  ros::shutdown();

  return 0;
}
