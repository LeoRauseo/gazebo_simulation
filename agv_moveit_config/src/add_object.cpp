#include "ros/ros.h"
#include "moveit/move_group_interface/move_group_interface.h"
#include "moveit/planning_scene_interface/planning_scene_interface.h"
#include "geometric_shapes/shape_operations.h"
#include "common_function.h"
#include <Eigen/Dense>
#include <tf2_ros/transform_listener.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/buffer.h>
#include <tf2/LinearMath/Transform.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <agv_moveit_config/variable.h>
int main(int argc, char **argv)
{



    int32_t desired_state=-1;
    bool use_fsm = false;
    ros::init(argc, argv, "add_object");
    ros::NodeHandle node_handle("~");



    node_handle.getParam("use_fsm", use_fsm);
    node_handle.getParam("state", desired_state);


    ROS_INFO("THE STATE IS %d",desired_state);
    ROS_INFO("THE STATE IS %d",desired_state);
    ROS_INFO("THE STATE IS %d",desired_state);ROS_INFO("THE STATE IS %d",desired_state);ROS_INFO("THE STATE IS %d",desired_state);ROS_INFO("THE STATE IS %d",desired_state);ROS_INFO("THE STATE IS %d",desired_state);ROS_INFO("THE STATE IS %d",desired_state);ROS_INFO("THE STATE IS %d",desired_state);




    ros::AsyncSpinner spinner(1);
    spinner.start();
    geometry_msgs::TransformStamped transformStamped;

    if(use_fsm) {
        ROS_INFO("TIM INSIDE THE IF");
        if(getState( desired_state,node_handle))
            ROS_INFO_NAMED("trial", "State Machine Working");
    }


    Eigen::Vector3d b(1, 1, 1);

    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;




    moveit_msgs::CollisionObject collision_object;
    collision_object.id = "cnc_machine";
    shapes::Mesh* m = shapes::createMeshFromResource("package://cnc_machine_gazebo/models/N_origin_tag.stl",b);
    ROS_INFO("Mesh loaded");

    shape_msgs::Mesh mesh;
    shapes::ShapeMsg mesh_msg;
    shapes::constructMsgFromShape(m, mesh_msg);
    mesh = boost::get<shape_msgs::Mesh>(mesh_msg);

    ROS_INFO("RUNNINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG");


    collision_object.meshes.resize(1);
    collision_object.mesh_poses.resize(1);
    collision_object.meshes[0] = mesh;
    collision_object.header.frame_id = "AT_pose";

       collision_object.mesh_poses[0].position.x = 0;
       collision_object.mesh_poses[0].position.y = 0;
       collision_object.mesh_poses[0].position.z = 0;
       collision_object.mesh_poses[0].orientation.z = 0;
       collision_object.mesh_poses[0].orientation.y = 0;
       collision_object.mesh_poses[0].orientation.x = 0;
       collision_object.mesh_poses[0].orientation.w = 0;


    collision_object.meshes.push_back(mesh);
    collision_object.mesh_poses.push_back(collision_object.mesh_poses[0]);
    collision_object.operation = collision_object.ADD;

    planning_scene_interface.applyCollisionObject(collision_object);
    ROS_INFO("cnc machine added into the world");


    if(use_fsm) {
            if(updateState(desired_state,node_handle))
                ROS_INFO_NAMED("trial", "State Machine Uptaded");
            else ROS_INFO_NAMED("trial", "State Machine ERROR state %d",desired_state);
    }



    sleep(2.0);



    ros::shutdown();




    return 0;
}
