var scn_data = {
	alarm: {
		alarm: 10,
		fault: 10
	},
	ros:{
		connect:"",
		voltage:0,
		mode:"normal",
		topic:""
	},
	uwb:{
		x:0,
		y:0
	},
	gyro:{
		yaw:0,
		roll:0,
		pitch:0
	},
	ultra:{
		fl:0,
		fr:0,
		bl:0,
		br:0
	},
	odom: {
		x: 0,
		y: 0,
		q: 0,
		vel: 0,
		rad: 0
	},
	uwb: {
		x: 0,
		y: 0
	},
	almMsg: [{
			msg: "AGV1号电量过低"
		},
		{
			msg: "AGV伺服电机使能失败"
		},
		{
			msg: "AGV点击过载保护"
		},
		{
			msg: "AGV传感器初始化失败"
		}
	],
	factoryHeader: [{
			"categories": "路线"
		},
		{
			"categories": "距离"
		},
		{
			"categories": "时间"
		},
		{
			"categories": "位置"
		},
		{
			"categories": "报警信息"
		},
		{
			"categories": "运行详情"
		}
	],
	factory: [{
			"company": "宝钢",
			"dtuCnt": 200,
			"plcCnt": 400,
			"dataCnt": 5000,
			"alarm": "无"
		},
		{
			"company": "造纸厂",
			"dtuCnt": 3000,
			"plcCnt": 2000,
			"dataCnt": 1000,
			"alarm": "无"
		},
		{
			"company": "锅炉厂",
			"dtuCnt": 1500,
			"plcCnt": 1000,
			"dataCnt": 500,
			"alarm": "无"
		}
	]
};
var vm = new Vue({
	el: '#content',
	data: scn_data,
	methods: {
		details: function () {

		}
	}
});
vm.$watch('scn_data', function () {
});
