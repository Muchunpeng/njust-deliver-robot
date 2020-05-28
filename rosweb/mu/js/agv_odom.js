
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
    vm.odom.x=odom.pose.pose.position.x.toFixed(3);
    vm.odom.y=odom.pose.pose.position.y.toFixed(3);
    vm.odom.q=odom.pose.pose.orientation;
    vm.odom.vel=odom.twist.twist.linear.x.toFixed(3);
    vm.odom.rad=odom.twist.twist.angular.z.toFixed(3);
    console.log("Success Receive Odom_Msg");
  }