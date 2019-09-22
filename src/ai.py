#!/usr/bin/env python
import math
import rospy
import time
from geometry_msgs.msg import Polygon
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Point32

# Initialize ROS
rospy.init_node("ai_node")

# Setup code
got_box_flag = False
boxes = []
def boxesCallback(msg):
    global boxes
    global got_box_flag
    boxes = msg.points
    got_box_flag = True
    #print("msg recieved")
    #print(boxes)

drone_pose = []
def dronePoseCallback(msg):

    global drone_pose
    drone_pose = msg.pose.position
    #print("msg recieved")
    #print(drone_pose)

def calculate_distance(p1, p2):
    return math.sqrt((p1.x-p2.x)**2 + ((p1.y-p2.y)**2))

def findClosestBox(boxes):
    #global boxes
    distance = 99999999
    closest_box = None
    for box in boxes:
        #print(box)
        temp_distance = math.sqrt((box.x-drone_pose.x)**2 + ((box.y-drone_pose.y)**2))
        if temp_distance < distance:
            distance = temp_distance
            closest_box = box
    #print(distance)
    print(closest_box)
    return closest_box

# Set up subscribers here
boxes_pose_sub = rospy.Subscriber("/perception/boxes", Polygon, boxesCallback)
drone_pose_sub = rospy.Subscriber("/mavros/local_position/pose", PoseStamped, dronePoseCallback)
box_pose_pub = rospy.Publisher("/control/position_setpoint", Point32, queue_size=1)

visited = []
# Main loop

#time.sleep(10000)
rospy.loginfo("ai is running")
rate = rospy.Rate(30)
while not rospy.is_shutdown():
    if got_box_flag:
        not_visited = [x for x in boxes if x not in visited]
        if not not_visited:
           visited[:] = [] 
           print("DONE")
           not_visited = [x for x in boxes if x not in visited]
        closest_box = findClosestBox(not_visited)
        box_pose_pub.publish(closest_box)
        while calculate_distance(drone_pose, closest_box) > 0.2:
            print(calculate_distance(drone_pose, closest_box))
            box_pose_pub.publish(closest_box)
        visited.append(closest_box)
    # Rate limiting
    rate.sleep()









