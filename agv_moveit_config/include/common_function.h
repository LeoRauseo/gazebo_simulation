//
// Created by leo on 23/06/02.
//

#ifndef AGV_MOVEIT_CONFIG_COMMON_FUNCTION_H
#define AGV_MOVEIT_CONFIG_COMMON_FUNCTION_H
#include <ros/ros.h>
#include <agv_moveit_config/variable.h>

inline bool getState(int& desired_state ,ros::NodeHandle& node_handle)
{

    ros::service::waitForService("/get_state_machine");
    int32_t actual_state=-1;

    try
    {


        // Read the variable value using a ROS service
        do
        {
            // Create a client to the get_variable service
            ros::ServiceClient getVariableClient = node_handle.serviceClient<agv_moveit_config::variable>(
                    "/get_state_machine");



            agv_moveit_config::variable::Request getRequest;
            agv_moveit_config::variable::Response getResponse;

            // Call the get_variable service and get the response
            if (getVariableClient.call(getRequest, getResponse))
            {
                if (getResponse.success)
                {
                    ROS_INFO_STREAM("Current variable value: " << getResponse.data);
                    actual_state = getResponse.data;
                }
                else
                {
                    ROS_WARN_STREAM("Failed to get variable value: " << getResponse.data);
                }
            }
            else
            {
                ROS_ERROR("Failed to call get_variable service");
                return false;
            }

            ros::Duration(0.5).sleep();
        } while (actual_state != desired_state);

        return true;
    }


    catch (const std::exception& e)
    {
        ROS_ERROR_STREAM("Service call failed: " << e.what());
        return false;
    }
}


inline bool updateState(int state,ros::NodeHandle& node_handle)
{

    ros::service::waitForService("/set_state_machine");
    try
    {


        // Create a client to the set_variable service
        ros::ServiceClient setVariableClient = node_handle.serviceClient<agv_moveit_config::variable>(
                "/set_state_machine");

        agv_moveit_config::variable::Request setRequest;
        agv_moveit_config::variable::Response setResponse;
        setRequest.data = state + 1;  // Example value to set the variable

        // Call the set_variable service and get the response
        if (setVariableClient.call(setRequest, setResponse))
        {
            if (setResponse.success)
            {
                ROS_INFO("Variable updated successfully");
                return true;
            }
            else
            {
                ROS_WARN_STREAM("Failed to update variable: " << setResponse.message);
                return false;
            }
        }
        else
        {
            ROS_ERROR("Failed to call set_variable service");
            return false;
        }
    }
    catch (const std::exception& e)
    {
        ROS_ERROR_STREAM("Service call failed: " << e.what());
        return false;
    }
}



#endif //AGV_MOVEIT_CONFIG_COMMON_FUNCTION_H
