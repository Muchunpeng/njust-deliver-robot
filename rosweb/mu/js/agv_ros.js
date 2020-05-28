//指定ROS服务器的IP地址和端口号
var ros = new ROSLIB.Ros();

//监听连接事件
ros.on('connection', function () {
  vm.ros.connect="Connected to websocket server.";
  console.log('Connected to websocket server.');
});
ros.on('error', function (error) {
  vm.ros.connect="Error connecting to websocket server.";
  console.log('Error connecting to websocket server: ', error);
});
ros.on('close', function () {
  vm.ros.connect="Connection to websocket server closed.";
  console.log('Connection to websocket server closed.');
});

function ros_connect() {
  //ros初始化，连接服务器
  var ros_ip = document.getElementById("ROS_IP").value;
  var ros_port = document.getElementById("ROS_PORT").value;
  ros.connect("ws:/" + ros_ip + ":" + ros_port);

}
