#include <math.h>
#include <ros/ros.h>
#include <iostream>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Point.h>
#include <fstream>
#include <string>
#include <vector>
#include <std_msgs/Int8.h>

using namespace ros;
using namespace std;

double pi = 3.14;

static double current_x;
static double current_y;

static double referenceLat;
static double referenceLong;

vector<double> obstacles_x;
vector<double> obstacles_y;

double next_wp_x;
double next_wp_y;

double threshold_distance = 5;

vector<double> wpx;
vector<double> wpy;

double feetToMt(double x)
{
   return (0.3048*x);
}

void getCurrent(const geometry_msgs::Point::ConstPtr& p)
{
   current_x = p->x;
   current_y = p->y;
}

geometry_msgs::Point toXY(double longitude, double latitude)
{
   double x = (longitude - referenceLong) * (40075000.0 / (2.0 * pi)) * cos(referenceLat);
   double y = (latitude - referenceLat) * (40007000.0 / (2.0 * pi));
   geometry_msgs::Point p;
   p.x = x;
   p.y = y;
   return p;
}

geometry_msgs::Point XY2LongLat(double xn, double yn)
{  
   double longit = (xn / cos(referenceLat)*(40075000.0 / (2.0 * pi))) + referenceLong; 
   double latit = (yn / (40007000.0 / (2.0 * pi))) + referenceLat; 
   geometry_msgs::Point p1;
   p1.x = longit;
   p1.y = latit;
   return p1;
}

double distance(double x1, double y1, double x2, double y2)
{
   return sqrt( pow((x1 - x2), 2) + pow((y1 - y2), 2) );
}

void referenceLong_callback(const std_msgs::Float64::ConstPtr& longref)
{
   referenceLong = longref->data;
}

void referenceLat_callback(const std_msgs::Float64::ConstPtr& latref)
{
   referenceLat = latref->data;
}

void readObstacleFile()
{  
   std::ifstream infile("/home/Desktop/Aurora/obstacles.txt");
   double lati, longi, r_in_feet;
   
   while (infile >> longi >> lati >> r_in_feet)
   {
      double r = feetToMt(r_in_feet);
      geometry_msgs::Point pt = toXY(longi,lati);
      double x = pt.x;
      double y = pt.y;

      obstacles_x.push_back(x);
      obstacles_y.push_back(y);
   }
}

void waypoint_callback(const geometry_msgs::Point::ConstPtr& pt)
{
   next_wp_x = pt->x;
   next_wp_y = pt->y;
}

void getpath(double start_x, double start_y, double end_x, double end_y)
{
   wpx.clear();
   wpy.clear();

   // find a path between both points convert points on that path to long lat and write (overwrite) in a txt file
} 

int main(int argc, char ** argv)
{
   ros::init(argc, argv, "planner");
   NodeHandle nh;
   Rate loop_rate(10);

   ros::Subscriber currentXY_sub = nh.subscribe("currentXY",10,getCurrent);
   ros::Subscriber referenceLong_Sub = nh.subscribe("referenceLong",10,referenceLong_callback);
   ros::Subscriber referenceLat_Sub = nh.subscribe("referenceLat",10,referenceLat_callback);
   ros::Subscriber waypoint_sub = nh.subscribe("next_waypoint",10,waypoint_callback);
   
   ros::Publisher condition_publisher = nh.advertise<std_msgs::Int8>("reached",10);

   readObstacleFile();

   while (ros::ok())
   {
      std_msgs::Int8 msg;
      if (distance(current_x, current_y, next_wp_x, next_wp_y) < threshold_distance)
      {
         msg.data = 0;
         condition_publisher.publish(msg);
      }
      else
      {
         msg.data = 1;
         condition_publisher.publish(msg);
         getpath(current_x, current_y, next_wp_x, next_wp_y);
      }
   }

}
