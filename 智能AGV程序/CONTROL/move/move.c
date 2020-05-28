
#include "move.h"
#include "key.h"
#include "servo.h"



/**************************************************************************
函数功能：运动解算和赋值
入口参数：车体速度、角速度
返回  值：无
**************************************************************************/
void Kinematic_Analysis(double Vel, double Pal)
{
    int32_t Vl = Speed_Conversion * (Vel - Pal * Wheel_Track / 2.0);
    int32_t Vr = -Speed_Conversion * (Vel + Pal * Wheel_Track / 2.0);

    SDO_Write(Lift_Wheel, Modes_Of_Operation, Velocity_Mode);
    SDO_Write(Right_Wheel, Modes_Of_Operation, Velocity_Mode);
	#if DEBUG_MODE
    printf("左 ：%d  		%X \r\n", Vl, Vl);
    printf("右 ：%d  		%X \r\n", Vr, Vr);
	printf("速度写入值：%d	%d\r\n",Vl,Vr);
	#else
	#endif
    if(Vel||Pal)
    {
        SDO_Write(my_agv.left_wheel.ID, Profile_Velocity, Vl);
        SDO_Write(my_agv.right_wheel.ID, Profile_Velocity, Vr);
        SDO_Write(Lift_Wheel, ControlWord, MOVE << 8 | Enable_Operate);
        SDO_Write(Right_Wheel, ControlWord, MOVE << 8 | Enable_Operate);

    }
    else
    {
        SDO_Write(Lift_Wheel, ControlWord, HALT << 8 | Enable_Operate);
        SDO_Write(Right_Wheel, ControlWord, HALT << 8 | Enable_Operate);
    }

}
/**************************************************************************
函数功能：按键控制车体运动
入口参数：无
返回  值：无
**************************************************************************/
void Car_Control()
{
    static float vel, pal;
    u8 key = KEY_Scan(0);
    if(key == KEY0_PRES)
        vel += 0.02f;

	#if DEBUG_MODE ==1
		if(ABS(my_agv.left_wheel.odom)>652000)
		{
			vel=0,pal=0;
		Kinematic_Analysis(vel, pal);	
		}
		printf("轮子里程计  %d\r\n",my_agv.left_wheel.odom);
	#endif
    if(key)
        Kinematic_Analysis(vel, pal);

		

}


