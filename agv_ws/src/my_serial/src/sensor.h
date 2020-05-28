#include <ros/ros.h>
#include <iostream>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/Imu.h>
#define PI 3.1415926535

typedef struct
{
    uint8_t ID, mode, enable, halt, nmt, status;
    int16_t tarV, actV;
    int32_t tarPos, actPos;
    float actCUR, actTOR;
    int32_t odom, last_odom, incre_odom;
} ServoMotor;

typedef struct
{
    uint8_t mode, status;
    float tarV, tarW, tarACC, tarDEC;
    int32_t tarPos;
    ServoMotor left_wheel, right_wheel;
    uint16_t q[4];
    uint16_t uwb[3];
    uint16_t undm[4];
} AGV_MODEL;


class my_odom 
{
public:
    my_odom();
    ~my_odom(){};
    void Odom_Release(AGV_MODEL my_agv);
    void Para_Init();
private:
    ros::NodeHandle n;
    ros::Publisher odom_pub;
    tf::TransformBroadcaster odom_broadcaster;
    double x;
    double y;
    double th;
    ros::Time current_time, last_time;
    std::string dev;
    double ODOM_FAC, VEL_FAC;
    double Wheel_Track,Wheel_Diameter;
    int Reduction_Ratio,Encoder_Accuracy,Velocity_Factor;
};

class my_undm
{
public:
    my_undm();
    ~my_undm(){};
    void Para_Init();
    int Obstacle_Check(uint16_t *dis);
private:
    ros::NodeHandle n;
    bool UNDM_Correct;
    int UNDM_Dis;
};
class my_imu 
{
public:
    my_imu();
    ~my_imu(){};
    void IMU_Release(uint16_t *q);
    void Para_Init();
private:
    ros::NodeHandle n;
    ros::Publisher imu_pub;
};
