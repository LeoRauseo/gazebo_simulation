
//Cpp
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

//ROS
#include "ros/ros.h"


// Include here the ".h" files corresponding to the topic type you use.
// ...
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>

#include <agv_moveit_config/variable.h>
#include "common_function.h"
// You may have a number of globals here.
//...

// Callback functions...


int main (int argc, char** argv)
{

        //ROS Initialization
    ros::init(argc, argv, "tf2_broadcaster_pic_tag");
    ros::NodeHandle node_handle("~");

    static tf2_ros::TransformBroadcaster br;
    geometry_msgs::TransformStamped transformStamped;



    bool use_fsm = false;
    node_handle.getParam("use_fsm", use_fsm);
    int32_t desired_state=2;



    if(use_fsm) {
        if(getState( desired_state,node_handle))
            ROS_INFO_NAMED("trial", "State Machine Working");
    }




    transformStamped.header.frame_id = "AT_pose";
    transformStamped.child_frame_id = "pic_tag";
    tf2::Quaternion q;
    transformStamped.transform.translation.x = 0.0;
    transformStamped.transform.translation.y = 0.0;
    transformStamped.transform.translation.z = 0.20;
    q.setRPY(M_PI,0 ,0);
    transformStamped.transform.rotation.x = q.x();
    transformStamped.transform.rotation.y = q.y();
    transformStamped.transform.rotation.z = q.z();
    transformStamped.transform.rotation.w = q.w();

    // Define your node handles: YOU NEED AT LEAST ONE !
    // ...

    // Read the node parameters if any
    // ...

    // Declare your node's subscriptions and service clients
    // ...

    // Declare you publishers and service servers
    // ...

    if(use_fsm) {
        if(updateState(desired_state,node_handle))
            ROS_INFO_NAMED("trial", "State Machine Uptaded");
        else ROS_INFO_NAMED("trial", "State Machine ERROR state %d",desired_state);
    }


    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.



        transformStamped.header.stamp = ros::Time::now();

        br.sendTransform(transformStamped);

        rate.sleep();
    }



}


