
#include "move.h"
#include "key.h"
#include "servo.h"



/**************************************************************************
�������ܣ��˶�����͸�ֵ
��ڲ����������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(double Vel, double Pal)
{
    int32_t Vl = Speed_Conversion * (Vel - Pal * Wheel_Track / 2.0);
    int32_t Vr = -Speed_Conversion * (Vel + Pal * Wheel_Track / 2.0);

    SDO_Write(Lift_Wheel, Modes_Of_Operation, Velocity_Mode);
    SDO_Write(Right_Wheel, Modes_Of_Operation, Velocity_Mode);
	#if DEBUG_MODE
    printf("�� ��%d  		%X \r\n", Vl, Vl);
    printf("�� ��%d  		%X \r\n", Vr, Vr);
	printf("�ٶ�д��ֵ��%d	%d\r\n",Vl,Vr);
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
�������ܣ��������Ƴ����˶�
��ڲ�������
����  ֵ����
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
		printf("������̼�  %d\r\n",my_agv.left_wheel.odom);
	#endif
    if(key)
        Kinematic_Analysis(vel, pal);

		

}


