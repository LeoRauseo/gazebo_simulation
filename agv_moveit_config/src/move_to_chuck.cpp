#include "ros/ros.h"
#include "common_function.h"
#include <moveit/move_group_interface/move_group_interface.h>
#include <geometry_msgs/Twist.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2/LinearMath/Transform.h>
#include <agv_moveit_config/variable.h>
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

  tf2::Matrix3x3 rot1(tf2::Quaternion(1,0,0,0));
  tf2::Vector3 translation1(0, 0, 0);
  tf2::Transform T_ATpose_tagRight(rot1, translation1);
  tf2::Matrix3x3 rot2(tf2::Quaternion(0,0,0,1));
  tf2::Vector3 translation2(-0.314, 0.212, -0.822); // tf data from tag_right to chuck
  tf2::Transform T_tagRight_chuck(rot2, translation2);

  tf2::Transform T_ATpose_chuck = T_ATpose_tagRight * T_tagRight_chuck;

  geometry_msgs::TransformStamped transformStamped;

  bool use_fsm = false;
  node_handle.getParam("use_fsm", use_fsm);

  int32_t desired_state=5;

  if(use_fsm) {
      if(getState( desired_state,node_handle))
          ROS_INFO_NAMED("trial", "State Machine Working");
  }



    static const std::string PLANNING_GROUP = "arm";
  moveit::planning_interface::MoveGroupInterface move_group_interface(PLANNING_GROUP);

    // class to add and remove collision objects in our "virtual world" scene
    // Raw pointers are frequently used to refer to the planning group for improved performance.

  const moveit::core::JointModelGroup *joint_model_group =
            move_group_interface.getCurrentState()->getJointModelGroup(PLANNING_GROUP);

    // We can print the name of the reference frame for this robot.
  ROS_INFO_NAMED("trial", "Planning frame: %s", move_group_interface.getPlanningFrame().c_str());

    // We can also print the name of the end-effector link for this group.
  ROS_INFO_NAMED("trial", "End effector link: %s", move_group_interface.getEndEffectorLink().c_str());

    // We can get a list of all the groups in the robot:
  ROS_INFO_NAMED("tutorial", "Available Planning Groups:");
  std::copy(move_group_interface.getJointModelGroupNames().begin(),
              move_group_interface.getJointModelGroupNames().end(),
              std::ostream_iterator<std::string>(std::cout, ", "));
  moveit::planning_interface::MoveGroupInterface::Plan my_plan;


    try {
        transformStamped = tfBuffer.lookupTransform("part_1", "AT_pose",
                                                    ros::Time(0));
    }
    catch (tf2::TransformException &ex) {
        ROS_WARN("%s", ex.what());
        ros::Duration(1.0).sleep();
        return 0;
    }
    tf2::Transform T_part1_ATpose;
    tf2::fromMsg(transformStamped.transform, T_part1_ATpose);



    tf2::Transform T_part1_chuck = T_part1_ATpose * T_ATpose_chuck;

    tf2::Quaternion q;
    q.setRPY(0, M_PI, 0);

    // Create the transformation matrix for the rotation because we want to look at the chuck
    tf2::Transform t;
    t.setRotation(q);
    t.setOrigin(tf2::Vector3(0, 0, 0));

    // in order to have the camera far away and the hand closer to the robot following tranformation
    tf2::Transform T_part1_facing_chuck= T_part1_chuck*t;
    q.setRPY(0, 0, -M_PI/2);
    t.setRotation(q);
    t.setOrigin(tf2::Vector3(0, 0, 0));
    T_part1_facing_chuck= T_part1_facing_chuck*t;
    // in order to move the hand as we want (z is facing the chuck, x is going up and y right if we look at the chuck
    q.setRPY(0, 0, 0);
    t.setRotation(q);
    t.setOrigin(tf2::Vector3(0, 0, -0.20));

    T_part1_facing_chuck= T_part1_facing_chuck*t;

    bool success=false;
    int count=0;


    tf2::Vector3 translation = T_part1_facing_chuck.getOrigin(); //translation
    tf2::Quaternion rotation = T_part1_facing_chuck.getRotation();  //rotation

    geometry_msgs::Pose target_pose;
    target_pose.position.x=translation.x();
    target_pose.position.y=translation.y();
    target_pose.position.z=translation.z();
    target_pose.orientation.x=rotation.x();
    target_pose.orientation.y=rotation.y();
    target_pose.orientation.z=rotation.z();
    target_pose.orientation.w=rotation.w();



    move_group_interface.setPoseTarget(target_pose,"camera_link_view");
    move_group_interface.setGoalOrientationTolerance(0.00001);
    move_group_interface.setGoalTolerance(0.01);



    do {
        count++;


          success = (move_group_interface.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
          ROS_INFO_NAMED("tutorial", "Visualizing plan 1 (pose goal) %s", success ? "" : "FAILED");
          move_group_interface.move();
          if (count==5) {

              success=true;
              ROS_INFO_NAMED("execution", "tried 5 with different rotation times but failed");
              continue;
          }

    }
    while(!success);

    if(use_fsm) {
        if(updateState(desired_state,node_handle))
            ROS_INFO_NAMED("trial", "State Machine Uptaded");
        else ROS_INFO_NAMED("trial", "State Machine ERROR state %d",desired_state);
    }
  ros::shutdown();

  return 0;
}
