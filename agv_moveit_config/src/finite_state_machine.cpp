#include <ros/ros.h>
#include <agv_moveit_config/variable.h>

class FiniteStateMachine
{
public:
    FiniteStateMachine()
    {
        variable_ = 0;
    }

    bool setVariable(agv_moveit_config::variable::Request& req, agv_moveit_config::variable::Response& res)
    {
        variable_ = req.data;
        res.success = true;
        res.message = "Variable updated successfully";
        res.data= variable_;
        return true;
    }

    bool getVariable(agv_moveit_config::variable::Request& req, agv_moveit_config::variable::Response& res)
    {
        res.success = true;
        res.message = "Variable got successfully";
        res.data= variable_;
        return true;
    }

private:
    int variable_;
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "finite_state_machine");
    ros::NodeHandle node_handle;

    FiniteStateMachine fsm;

    ros::ServiceServer setVariableService = node_handle.advertiseService("/set_state_machine", &FiniteStateMachine::setVariable, &fsm);
    ros::ServiceServer getVariableService = node_handle.advertiseService("/get_state_machine", &FiniteStateMachine::getVariable, &fsm);

    ros::spin();

    return 0;
}

