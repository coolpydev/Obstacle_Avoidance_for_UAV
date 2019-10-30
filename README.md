# Obstacle Avoidance Simulator for Unmanned Aerial Vehicles (UAVs)
This is a ROS workspace that creates a trajectory for a UAV to follow passing through a set of given waypoints and avoiding a set of given cylindrical obstacles, using a path planning algorithm. The testing is done through a node which plots the waypoints, obstacles and the current pose of UAV on RVIZ for examining the accuracy of the algorithm. 

# Requirements :
1. ROS 
2. ardupilot
3. mavros
4. rviz
5. Mission Planner (preferred) or apm planner

# Commands : 

## Testing existing Algorithms :
```
1. roscore
2. /(path to sim_vehicle)/sim_vehicle.py --console --map --aircraft test
3. roslaunch mavros apm2.launch fcu_url:=udp://localhost:14550@ 
4. rosrun map currentXY  
5. rosrun map markPoints
6. rviz 
(write the frame id i.e. /my_frame in the Fixed Frame)
7. rosrun tf static_transform_publisher 0 0 0 0 0 0 1 map my_frame 10
8. rosrun map waypoints
```


For any queries, please contact me via mail on sarthak16189@iiitd.ac.in

new edits: the path planner is now using better params for global cost map.
