
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

// You may have a number of globals here.
//...

// Callback functions...


int main (int argc, char** argv)
{

        //ROS Initialization
    ros::init(argc, argv, "tf2_broadcaster_gr5");

    static tf2_ros::TransformBroadcaster br;
    geometry_msgs::TransformStamped transformStamped;
    transformStamped.header.frame_id = "assembly_1";
    transformStamped.child_frame_id = "frame_5";
    tf2::Quaternion q;
    transformStamped.transform.translation.x = 0.0;
    transformStamped.transform.translation.y = 0.0;
    transformStamped.transform.translation.z = 0.1;
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

    ros::Rate rate(10);   // Or other rate.
    while (ros::ok()){
        ros::spinOnce();

        // Your node's code goes here.



        transformStamped.header.stamp = ros::Time::now();

        br.sendTransform(transformStamped);

        rate.sleep();
    }
}


