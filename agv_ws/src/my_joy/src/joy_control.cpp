
#include "joy_control.h"

using namespace std;

Teleop::Teleop() {
pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  sub = n.subscribe<sensor_msgs::Joy>("/joy", 1, &Teleop::callback, this);
  this->Para_Init();
}

void Teleop::callback(const sensor_msgs::Joy::ConstPtr &Joy) {

  if (Joy->buttons[KEY_R2]) {
    if (Joy->buttons[KEY_ONE])
      v.linear.x += vlinear;
    if (Joy->buttons[KEY_TWO])
      v.angular.z -= vangular;
    if (Joy->buttons[KEY_THREE])
      v.linear.x -= vlinear;
    if (Joy->buttons[KEY_FOUR])
      v.angular.z += vangular;
  } else {
    v.linear.x = 0;
    v.angular.z = 0;
  }
 
   pub.publish(v);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "joy_control");
  Teleop telelog;
  ros::spin();
  return 0;
}
void Teleop::Para_Init() {
  n.param<int>("joy_control/axis_linear", axis_lin, 1);
  n.param<int>("joy_control/axis_angular", axis_ang, 0);
  n.param<double>("joy_control/vel_linear", vlinear, 0.01);
  n.param<double>("joy_control/vel_angular", vangular, 0.01);
  std::cout << "Linear is " << vlinear << std::endl;
  std::cout << "Angular is " << vangular << std::endl;
}
