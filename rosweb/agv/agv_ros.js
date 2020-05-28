//ros初始化，连接服务器
var ros_ip="/muagv.wicp.vip";
var ros_port="100";
 //指定ROS服务器的IP地址和端口号
 var ros = new ROSLIB.Ros();
 ros.connect("ws:/"+ros_ip+":"+ros_port);
 //监听连接事件
 ros.on('connection', function () {
   console.log('Connected to websocket server.');
 });
 ros.on('error', function (error) {
   console.log('Error connecting to websocket server: ', error);
 });
 ros.on('close', function () {
   console.log('Connection to websocket server closed.');
 });
