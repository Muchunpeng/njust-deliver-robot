//接收AGV的里程计数据
var odom_x_edit=document.getElementById("odom_x");
var odom_y_edit=document.getElementById("odom_y");
var odom_q_edit=document.getElementById("odom_q");
var odom_vel_edit=document.getElementById("odom_vel");
var odom_rad_edit=document.getElementById("odom_rad");
// Subscribing to a Topic
 // ----------------------
 //订阅一个话题，具有回调函数
 var vel_sub = new ROSLIB.Topic({
    ros: ros,
    name: '/odom',
    messageType: 'nav_msgs/Odometry'
  });
  vel_sub.subscribe(odom_callback);
  function odom_callback(odom)
  {
    odom_x_edit.innerHTML=odom.pose.pose.position.x;
    odom_y_edit.innerHTML=odom.pose.pose.position.y;
    odom_q_edit.innerHTML=odom.pose.pose.orientation;
    odom_vel_edit.innerHTML=odom.twist.twist.linear.x;
    odom_rad_edit.innerHTML=odom.twist.twist.angular.z;
    console.log("Success Receive Odom_Msg");
  }