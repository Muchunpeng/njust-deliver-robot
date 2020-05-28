
#include "serial.h"

using namespace std;

int main(int argc, char **argv) {
  ros::init(argc, argv, "serial_port");
  //创建一个serial类
  my_serial sp;
  
  ros::Rate loop_rate(100);
  while (ros::ok()) {
    //获取缓冲区内的字节数
    size_t n = sp.available();
    if (n) {
      uint8_t buffer[12];
      //读出数据
      n = sp.read(buffer, n);
    sp.Data_Decode(buffer);   
    }
    loop_rate.sleep();
    ros::spinOnce();
  }
  sp.close();
  return 0;
}

void my_serial::Para_Init() {
  n.param<int>("ipc_serial/Baud_Rate", baudrate, 115200);
  n.param<std::string>("ipc_serial/dev", dev, "/dev/ttyUSB0");
  std::cout << "Port is " << dev << std::endl;
  std::cout << "Baud Rate is " << baudrate << std::endl;
}

uint8_t my_serial::Data_Decode(uint8_t *msg) {
  if (msg[0] == IPC_RES) {
    switch (msg[2]) {
    case VEL_res:
      memcpy(&my_agv.left_wheel.actV, &msg[3], 2);
      memcpy(&my_agv.right_wheel.actV, &msg[7], 2);
      break;
    case Odom_res:
      memcpy(&my_agv.left_wheel.incre_odom, &msg[3], 4);
      memcpy(&my_agv.right_wheel.incre_odom, &msg[7], 4);
      odom.Odom_Release(my_agv);
      break;
    case Gyro_req:
      memcpy(&my_agv.q, &msg[3], 8);
      imu.IMU_Release(my_agv.q);
      break;
    case UWB_req:
      memcpy(&my_agv.uwb, &msg[3], 6);
      break;
    case UNDM_req:
      memcpy(&my_agv.undm, &msg[3], 8);
      undm.Obstacle_Check(my_agv.undm);
      break;
    default:
      break;
    }
    return msg[2];
  }
  return 0;
}
void my_serial::vel_Callback(const geometry_msgs::Twist &cmd_vel) {
  // ROS_INFO("Linear :[%f] Angular :[%f]", cmd_vel.linear.x,
  // cmd_vel.angular.z);
  uint8_t tx_buf[12];
  tx_buf[0] = IPC_SET;
  tx_buf[2] = VEL_set;
  float vel = cmd_vel.linear.x;
  float pal = cmd_vel.angular.z;
  memcpy(&tx_buf[3], &vel, 4);
  memcpy(&tx_buf[7], &pal, 4);
  ROS_INFO("VEL:[%f] PAL:[%f] ", vel, pal);
  write(tx_buf, 12);
}

my_serial::my_serial() {
  vel_sub = n.subscribe("/cmd_vel", 1, &my_serial::vel_Callback, this);
  this->Para_Init();
  serial::Timeout to = serial::Timeout::simpleTimeout(100);
  //设置要打开的串口名称
  this->setPort(dev);
  //设置串口通信的波特率
  this->setBaudrate(baudrate);
  //串口设置timeout
  this->setTimeout(to);

  try {
    //打开串口
    this->open();
  } catch (serial::IOException &e) {
    ROS_ERROR_STREAM("Unable to open port.");
  }

  //判断串口是否打开成功
  if (this->isOpen()) {
    ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
  }
}