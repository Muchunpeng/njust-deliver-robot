 #include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include<iostream>

#define KEY_ONE     0
#define KEY_TWO     1
#define KEY_THREE   2
#define KEY_FOUR    3
#define KEY_L2      4 
#define KEY_R2      5
#define KEY_L1      6
#define KEY_R1      7
class Teleop
{
public:
    Teleop();
    void Para_Init();
private:
    /* data */
    void callback(const sensor_msgs::Joy::ConstPtr& Joy);
    ros::NodeHandle n;
    ros::Subscriber sub ;
    ros::Publisher pub ;
    double vlinear,vangular;
    int axis_ang,axis_lin,ton;
    geometry_msgs::Twist v;
};