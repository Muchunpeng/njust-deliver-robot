var option1 = {
    tooltip: {
        formatter: "{a} <br/>{b} : {c}"
    },
    toolbox: {
        show: false,
        feature: {
            mark: {
                show: true
            },
            restore: {
                show: true
            },
            saveAsImage: {
                show: false
            }
        }
    },
    series: [{
        name: '速度',
        type: 'gauge',
        min: -0.5,
        max: 0.5,
        splitNumber: 4, // 分割段数，默认为5
        axisLine: { // 坐标轴线
            lineStyle: { // 属性lineStyle控制线条样式
                color: [
                    [0.2, '#228b22'],
                    [0.8, '#48b'],
                    [1, '#ff4500']
                ],
                width: 4
            }
        },
        axisTick: { // 坐标轴小标记
            splitNumber: 5, // 每份split细分多少段
            length: 6, // 属性length控制线长
            lineStyle: { // 属性lineStyle控制线条样式
                color: 'auto'
            }
        },
        axisLabel: { // 坐标轴文本标签，详见axis.axisLabel
            textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                color: 'auto'
            }
        },
        splitLine: { // 分隔线
            show: true, // 默认显示，属性show控制显示与否
            length: 10, // 属性length控制线长
            lineStyle: { // 属性lineStyle（详见lineStyle）控制线条样式
                color: 'auto'
            }
        },
        pointer: {
            width: 4
        },
        title: {
            show: true,
            offsetCenter: [0, '-40%'], // x, y，单位px
            textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                color: 'auto',
                fontWeight: 'bolder'
            }
        },
        detail: {
            formatter: '{value}',
            offsetCenter: [0, '80%'], // x, y，单位px
            textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                color: 'auto',
                fontWeight: 'bolder'
            }
        },
        data: [{
            value: 0,
            name: 'V'
        }]
    }]
};
var option2 = {
    tooltip: {
        formatter: "{a} <br/>{b} : {c}%"
    },
    toolbox: {
        show: false,
        feature: {
            mark: {
                show: true
            },
            restore: {
                show: true
            },
            saveAsImage: {
                show: false
            }
        }
    },
    series: [{
        name: '角速度',
        type: 'gauge',
        min: -0.5,
        max: 0.5,
        splitNumber: 4, // 分割段数，默认为5
        axisLine: { // 坐标轴线
            lineStyle: { // 属性lineStyle控制线条样式
                color: [
                    [0.2, '#228b22'],
                    [0.8, '#48b'],
                    [1, '#ff4500']
                ],
                width: 4
            }
        },
        axisTick: { // 坐标轴小标记
            splitNumber: 5, // 每份split细分多少段
            length: 6, // 属性length控制线长
            lineStyle: { // 属性lineStyle控制线条样式
                color: 'auto'
            }
        },
        axisLabel: { // 坐标轴文本标签，详见axis.axisLabel
            textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                color: 'auto'
            }
        },
        splitLine: { // 分隔线
            show: true, // 默认显示，属性show控制显示与否
            length: 10, // 属性length控制线长
            lineStyle: { // 属性lineStyle（详见lineStyle）控制线条样式
                color: 'auto'
            }
        },
        pointer: {
            width: 4
        },
        title: {
            show: true,
            offsetCenter: [0, '-40%'], // x, y，单位px
            textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                color: 'auto',
                fontWeight: 'bolder'
            }
        },
        detail: {
            formatter: '{value}',
            offsetCenter: [0, '80%'], // x, y，单位px
            textStyle: { // 其余属性默认使用全局文本样式，详见TEXTSTYLE
                color: 'auto',
                fontWeight: 'bolder'
            }
        },
        data: [{
            value: 0,
            name: 'W'
        }]
    }]
};
// 基于准备好的dom，初始化echarts图表
var dash1 = echarts.init(document.getElementById('container'));
var dash2 = echarts.init(document.getElementById('container1'));
// 为echarts对象加载数据 
dash1.setOption(option1);
dash2.setOption(option2);

setInterval(function () {
    option1.series[0].data[0].value = vm.odom.vel;
    option2.series[0].data[0].value = vm.odom.rad;
    dash1.setOption(option1, true);
    dash2.setOption(option2, true);
}, 500);

// vm.$watch('odom.vel', function () {
//     option1.series[0].data[0].value = vm.odom.vel;
//     dash1.setOption(option1, true);
// });
// vm.$watch('odom.rad', function () {
//     option2.series[0].data[0].value = vm.odom.rad;
//     dash2.setOption(option2, true);
// });