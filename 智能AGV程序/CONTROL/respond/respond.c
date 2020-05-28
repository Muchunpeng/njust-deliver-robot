
#include "respond.h"
#include "usart.h"
#include "servo.h"
/**************************************************************************
函数功能：工控机设置
入口参数：接收信息数组，agv模型结构体
返回  值：成功：1
		　失败：0
备注：
**************************************************************************/
void IPC_Set(u8 *msg, AGV_MODEL *agv)
{
    switch(msg[2])
    {
    case(Drive_set):
        if(msg[1] == Lift_Wheel)
        {
            agv->left_wheel.nmt = msg[3];
            agv->left_wheel.mode = msg[4];
            agv->left_wheel.enable = msg[5];
            agv->left_wheel.halt = msg[6];
        }
        else if(msg[1] == Right_Wheel)
        {
            agv->right_wheel.nmt = msg[3];
            agv->right_wheel.mode = msg[4];
            agv->right_wheel.enable = msg[5];
            agv->right_wheel.halt = msg[6];
        }
        break;
    case(VEL_set):
        memcpy(&agv->tarV, msg + 3, 4);
        memcpy(&agv->tarW, msg + 7, 4);
        Kinematic_Analysis(agv->tarV, agv->tarW);
        break;
    case(ACC_set):
        memcpy(&agv->tarACC, msg + 3, 4);
        memcpy(&agv->tarDEC, msg + 7, 4);
        break;
    case(SITE_set):
        agv->mode = msg[3];
        memcpy(&agv->tarPos, msg + 7, 4);
        break;
    default:
        break;
    }
    IPC_Req(msg[2], msg, agv);
}
/**************************************************************************
函数功能：响应IPC返回
入口参数：指令，发送数组，agv模型
返回  值：无
备注：

**************************************************************************/
void IPC_Req(u8 cmd, u8 *msg, AGV_MODEL *agv)
{
    msg[0] = IPC_RES;
    msg[2] = cmd;
    switch(cmd)
    {
    case(VEL_req):
        if(msg[1] == Lift_Wheel)
        {
            memcpy(msg + 3, &agv->left_wheel.actV, 4);
            memcpy(msg + 7, &agv->left_wheel.odom, 4);
        }
        else if(msg[1] == Right_Wheel)
        {
            memcpy(msg + 3, &agv->right_wheel.actV, 4);
            memcpy(msg + 7, &agv->right_wheel.odom, 4);
        }
        break;
    case(Current_req):
        if(msg[1] == Lift_Wheel)
        {
            memcpy(msg + 3, &agv->left_wheel.actCUR, 2);
            memcpy(msg + 5, &agv->left_wheel.actTOR, 2);
        }
        if(msg[1] == Right_Wheel)
        {
            memcpy(msg + 3, &agv->right_wheel.actCUR, 2);
            memcpy(msg + 5, &agv->right_wheel.actTOR, 2);
        }
        break;
    case(Gyro_req):
        memcpy(msg + 3, agv->q, 8);
        break;
    case(UWB_req):
        memcpy(msg + 3, agv->uwb, 6);
        break;
    case(UNDM_req):
        memcpy(msg + 3, agv->undm, 8);
        break;
    default:
        break;
    }
    AGV_SendMsg(msg, 12);
}
/**************************************************************************
函数功能：NMT模式选择
入口参数：ID，NMT模式
返回  值：无
**************************************************************************/
void IPC_Res(u8 cmd, u8 *msg, AGV_MODEL *agv)
{
    msg[0] = IPC_RES;
    msg[2] = cmd;
    switch(cmd)
    {
    case(Heart_Check):
        break;
    case(Error_Alarm):
        break;
    case(VEL_res):
        memcpy(msg + 3, &agv->left_wheel.actV, 2);
        memcpy(msg + 7, &agv->right_wheel.actV, 2);
        break;
    case(Odom_res):
        agv->left_wheel.incre_odom = agv->left_wheel.odom - agv->left_wheel.last_odom;
        agv->right_wheel.incre_odom = agv->right_wheel.odom - agv->right_wheel.last_odom;
        agv->left_wheel.last_odom = agv->left_wheel.odom;
        agv->right_wheel.last_odom = agv->right_wheel.odom;
        memcpy(msg + 3, &agv->left_wheel.incre_odom, 4);
        memcpy(msg + 7, &agv->right_wheel.incre_odom, 4);
        break;
    default :
        break;
    }
    AGV_SendMsg(msg, 12);
}




