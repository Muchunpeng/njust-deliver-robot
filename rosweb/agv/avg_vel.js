//键盘控制小车移动
//使用文本框调整速度
 var tar_vel_edit=document.getElementById("vel");
 var tar_rad_edit=document.getElementById("rad");

 var twist = new ROSLIB.Message({
    linear : {
      x : 0,
      y : 0,
      z : 0
    },
    angular : {
      x : 0,
      y : 0,
      z : 0
    }
  });

 var cmdVel = new ROSLIB.Topic({
    ros : ros,
    name : '/cmd_vel',
    messageType : 'geometry_msgs/Twist'
 });
 function send_cmdvel()
 {
  cmdVel.publish(twist);
 }

