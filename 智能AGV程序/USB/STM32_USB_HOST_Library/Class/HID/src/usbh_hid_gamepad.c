#include "usbh_hid_gamepad.h"
#include "usart.h"


//FC游戏手柄模拟
//读取fcpad.ctrlval,即可得到当前手柄格式转换成FC手柄格式.
FC_GamePad_TypeDef fcpad;
FC_GamePad_TypeDef fcpad1;


static void  GAMEPAD_Init (void);
static void  GAMEPAD_Decode(u8	*data);


HID_cb_TypeDef HID_GAMEPAD_cb =
{
    GAMEPAD_Init,
    GAMEPAD_Decode,
};

//game pad 初始化
static void  GAMEPAD_Init ( void)
{
    USR_GAMEPAD_Init();
}

//通用USB方向键解码,结果存放在fcpad里面
//data0,data1:USB得到的方向数据
//数据格式如下:
//data0:00,左键按下;ff,右键按下,7F,没有按键按下
//data1:00,上键按下;ff,下键按下,7F,没有按键按下
void GAMEPAD_nDir_Decode(u8 data0, u8 data1)
{
    switch(data0)
    {
    case 0X7F:
        fcpad.left = 0;
        fcpad.right = 0;
        break;
    case 0X00:
        fcpad.left = 1;
        break;
    case 0XFF:
        fcpad.right = 1;
        break;
    }
    switch(data1)
    {
    case 0X7F:
        fcpad.up = 0;
        fcpad.down = 0;
        break;
    case 0X00:
        fcpad.up = 1;
        break;
    case 0XFF:
        fcpad.down = 1;
        break;
    }
}
//通用USB AB键解码,结果存放在fcpad里面
//data:USB得到的1/2/3/4键数据
//数据格式如下:
//data:最高4位有效
//b4=1,1键按下(FC:B键连按)
//b5=1,2键按下(FC:A键连按)
//b6=1,3键按下(FC:A键)
//b7=1,4键按下(FC:B键)
void GAMEPAD_nAB_Decode(u8 data)
{
    if(data & 0X10)
        fcpad.a = 1;	//A键按下
    else
        fcpad.a = 0;	//A键按下
    if(data & 0X20)
        fcpad.b = 1;	//B键按下
    else
        fcpad.b = 0;	//B键按下
    if(data & 0X40)
        fcpad.c = 1;	//C键按下
    else
        fcpad.c = 0;	//C键按下
    if(data & 0X80)
        fcpad.d = 1;	//D键按下
    else
        fcpad.d = 0;	//D键按下
}
//通用USB 功能键解码,结果存放在fcpad里面
//data:USB得到的Select/Start键数据
//数据格式如下:
//data:b4,b5有效.
//b4=1,Select键按下
//b5=1,Start键按下
void GAMEPAD_nFun_Decode(u8 data)
{
    if(data & 0X10)fcpad.select = 1;	//Select键按下
    else fcpad.select = 0;			//Select键松开
    if(data & 0X20)fcpad.start = 1;	//Start键按下
    else fcpad.start = 0;			//Start键松开

    if(data & 0X04)fcpad.l1 = 1;	//L1键按下
    else fcpad.l1 = 0;			//L1键松开
    if(data & 0X01)fcpad.l2 = 1;	//L2键按下
    else fcpad.l2 = 0;			//L2键松开
    if(data & 0X08)fcpad.r1 = 1;	//R1键按下
    else fcpad.r1 = 0;			//R1键松开
    if(data & 0X02)fcpad.r2 = 1;	//R2键按下
    else fcpad.r2 = 0;			//R2键松开
}

//game pad数据解析
static void  GAMEPAD_Decode(uint8_t *data)
{
    {
        GAMEPAD_nDir_Decode(data[0], data[1]);	//解码方向键
        GAMEPAD_nAB_Decode(data[5]);			//解码AB键
        GAMEPAD_nFun_Decode(data[6]);			//解码功能键
    }
    if(fcpad.up)printf("上\n");
    if(fcpad.right)printf("右\n");
    if(fcpad.down)printf("下\n");
    if(fcpad.left)printf("左\n");
    if(fcpad.select)printf("选择\n");
    if(fcpad.start)printf("开始\n");
    if(fcpad.a)printf("A\n");
    if(fcpad.b)printf("B\n");
    if(fcpad.c)printf("C\n");
    if(fcpad.d)printf("D\n");
    if(fcpad.l1)printf("L1\n");
    if(fcpad.l2)printf("L2\n");
    if(fcpad.r1)printf("R1\n");
    if(fcpad.r2)printf("R2\n");

}


















