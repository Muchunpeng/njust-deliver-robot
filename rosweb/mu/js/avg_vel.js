//键盘控制小车移动
//使用文本框调整速度
var tar_vel_edit = document.getElementById("vel");
var tar_rad_edit = document.getElementById("rad");

var twist = new ROSLIB.Message({
  linear: {
    x: 0,
    y: 0,
    z: 0
  },
  angular: {
    x: 0,
    y: 0,
    z: 0
  }
});

var cmdVel = new ROSLIB.Topic({
  ros: ros,
  name: '/cmd_vel',
  messageType: 'geometry_msgs/Twist'
});

function send_cmdvel() {
  cmdVel.publish(twist);
}

function whichButton(event) {
  if (event.ctrlKey) {
    switch (event.keyCode) {
      case 38:
          twist.linear.x+=0.01;
      break;
      case 40:
          twist.linear.x+=0.01;
      break;
      case 37:
          twist.angular.z += 0.01;
      break;
      case 39:
          twist.angular.z -= 0.01;
      break;
      default:
        twist.linear.x = 0;
        twist.angular.z = 0;
        break;
    }

  } else {
    twist.linear.x = 0;
    twist.angular.z = 0;
  }
  cmdVel.publish(twist);
  event.s
}
