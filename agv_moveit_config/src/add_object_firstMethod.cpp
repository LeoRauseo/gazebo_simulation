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


    int count=0;
    int32_t desired_state=-1;
    bool use_fsm = true;
    bool twice=true;
    while (twice && use_fsm ){

        use_fsm=false;
        twice=false;
        ros::init(argc, argv, "add_object");


        tf2_ros::Buffer tfBuffer;
        tf2_ros::TransformListener tfListener(tfBuffer);
        ros::NodeHandle node_handle;


        node_handle.getParam("use_fsm", use_fsm);

        ros::AsyncSpinner spinner(1);
        spinner.start();
        geometry_msgs::TransformStamped transformStamped;


        if(use_fsm) {
            twice=true;
            if(count==0)
                desired_state=1;
            if(count==1)
                desired_state=4;

            if(getState( desired_state,node_handle))
                    ROS_INFO_NAMED("trial", "State Machine Working");
            count++;


        if (desired_state == 4)
            twice = false;

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




    try {

       transformStamped = tfBuffer.lookupTransform("part_1", "AT_pose", ros::Time(0), ros::Duration(20));
    }
    catch (tf2::TransformException &ex) {
        ROS_WARN("%s", ex.what());
        ros::Duration(1.0).sleep();
        return 0;
    }

    tf2::Transform transform;
    tf2::fromMsg(transformStamped.transform, transform);



    // Apply a -90 degree rotation around the x-axis
    tf2::Quaternion q_x;
    q_x.setRPY(-M_PI/2, 0, 0);
    transform.setRotation(transform.getRotation() * q_x);
// Apply a 90 degree rotation around the z-axis
    tf2::Quaternion q_z;
    q_z.setRPY(0, 0, M_PI/2);
    transform.setRotation(transform.getRotation() * q_z);

    tf2::Vector3 translation = transform.getOrigin();
    tf2::Quaternion rotation = transform.getRotation();

    std::cout << transformStamped.transform.translation.x << "   " << transformStamped.transform.translation.y << "   " << transformStamped.transform.translation.z << std::endl;

    collision_object.meshes.resize(1);
    collision_object.mesh_poses.resize(1);
    collision_object.meshes[0] = mesh;
    collision_object.header.frame_id = "part_1";

       collision_object.mesh_poses[0].position.x = translation.x();
       collision_object.mesh_poses[0].position.y = translation.y();
       collision_object.mesh_poses[0].position.z = translation.z();
       collision_object.mesh_poses[0].orientation.z = rotation.z();
       collision_object.mesh_poses[0].orientation.y = rotation.y();
       collision_object.mesh_poses[0].orientation.x = rotation.x();
       collision_object.mesh_poses[0].orientation.w = rotation.w();

    /*     -0.517, -0.459, 1.392


         collision_object.mesh_poses[0].position.x =  -0.517;
         collision_object.mesh_poses[0].position.y = -0.459;
         collision_object.mesh_poses[0].position.z = 1.392;
         collision_object.mesh_poses[0].orientation.z = 1;
         collision_object.mesh_poses[0].orientation.y = 0;
         collision_object.mesh_poses[0].orientation.x = 0;
         collision_object.mesh_poses[0].orientation.w = 0;*/
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

    }


    return 0;
}
