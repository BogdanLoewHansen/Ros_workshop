#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Polygon.h>
#include <geometry_msgs/Point32.h>
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
// Define callbacks here
geometry_msgs::PoseArray boxmsg;
geometry_msgs::Polygon polygonmsg;

void callback(const geometry_msgs::PoseArray& msg) {
  boxmsg = msg;
  geometry_msgs::Point32 p;
  //ROS_INFO(msg.size)
  for(int i = 0; i < msg.poses.size(); i++){
      float x = static_cast<float>(boxmsg.poses[i].position.x);
      float y = static_cast<float>(boxmsg.poses[i].position.y);
      float z = static_cast<float>(boxmsg.poses[i].position.z);
      p.x = x;
      p.y = y;
      p.z = z;
      polygonmsg.points.push_back(p);
  }

  //ROS_INFO("Message recieved"); // debug print
}

int main(int argc, char** argv){
    // Initialise ROS
    ros::init(argc, argv, "perception_node");
    ros::NodeHandle n;

    // Set up subscribers/publishers here
    ros::Publisher pub = n.advertise<geometry_msgs::Polygon>("perception/boxes", 1);
    ros::Subscriber sub = n.subscribe("/simulator/boxes", 1, callback);



    // Main loop
    ROS_INFO("perception is running");
    ros::Rate rate(30);
    while (ros::ok()) {

      pub.publish(polygonmsg);
      rate.sleep();
      ros::spinOnce();
    }

    return 0;
}


