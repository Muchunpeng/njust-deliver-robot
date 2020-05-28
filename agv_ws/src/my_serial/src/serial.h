#include "sensor.h"
#include <serial/serial.h>
#define VEL_set 0x02

#define Current_req 		0x06
#define Gyro_req 			0x07
#define UWB_req 			0x08
#define UNDM_req 			0x09

#define VEL_res 0x12
#define Odom_res 0x13
#define IPC_SET 0xFF
#define IPC_REQ 0xFC
#define IPC_RES 0xFE


class my_serial : public serial::Serial
{
public:
    my_serial();
    ~my_serial(){};
    void vel_Callback(const geometry_msgs::Twist& cmd_vel);
    uint8_t Data_Decode(uint8_t* msg);
    void Para_Init();
    AGV_MODEL my_agv;

private:
    ros::NodeHandle n;
    ros::Subscriber vel_sub;
    std::string dev;
    int baudrate;
    my_odom odom;
    my_undm undm;
    my_imu imu;
};
