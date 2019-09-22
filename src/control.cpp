#include <iostream>
#include <ros/ros.h>
#include "mavros_msgs/PositionTarget.h"
#include "std_msgs/Float64.h"
#include <geometry_msgs/Point32.h>

mavros_msgs::PositionTarget mavros_set_point;

void callback(const geometry_msgs::Point32& msg) {
    double x = msg.x;//static_cast<std_msgs::Float64>(msg.x);
    double y = msg.y;//static_cast<std_msgs::Float64>(msg.y);
    double z = 3.0;
    mavros_set_point.position.x = x;
    mavros_set_point.position.y = y;
    mavros_set_point.position.z = z;
  }

int main(int argc, char** argv){
    // Initialise ROS
    ros::init(argc, argv, "perception_node");
    ros::NodeHandle n;

    std::cout << "control is running" << std::endl;

    // Set up subscribers/publishers here
    ros::Publisher pub = n.advertise<mavros_msgs::PositionTarget>("/mavros/setpoint_raw/local", 1);
    ros::Subscriber sub = n.subscribe("/control/position_setpoint", 1, callback);

    ros::Rate rate(30);
    while (ros::ok()) {

      pub.publish(mavros_set_point);
      rate.sleep();
      ros::spinOnce();
    }

    return 0;
}
