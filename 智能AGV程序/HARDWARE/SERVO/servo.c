#include "servo.h"
#include "can.h"

ServoMotor left_wheel = {1};
ServoMotor right_wheel = {2};

/**************************************************************************
�������ܣ�NMTģʽѡ��
��ڲ�����ID��NMTģʽ
����  ֵ���ɹ���0
		��ʧ�ܣ�1
��ע��
//����ģʽ��	0x01 ����0x05
//Ԥ����ģʽ��	0x80 ����0x7F
//�ڲ���ģʽ�»�ʹ��PDO
**************************************************************************/
u8 Servo_Mode(u16 id, u8 mode)
{
    u8 msg[2];
    msg[0] = mode;
    msg[1] = id;
    return CAN1_Send_Msg(NMT_Manage, msg, 2);
}


/**************************************************************************
�������ܣ�SDOд����
��ڲ�����ID������������
����  ֵ���ɹ���1
		��ʧ�ܣ�0
��ע��
**************************************************************************/
u8 SDO_Write(u16 id, u16 index, int32_t data)
{
    OS_ERR err;
    u8 msg[8] = {0};
    id = SDO_WRITE + id;
    msg[0] = Select_Len(index, SDO_WRITE);
    msg[1] = index & 0xFF;
    msg[2] = (index >> 8) & 0xFF;
    msg[3] = NULL;
    memcpy(msg + 4, &data, 4);
    while(CAN1_Send_Msg(id, msg, 8));
    OSSemPend(&CAN_SEM, CAN_WaitTime, OS_OPT_PEND_BLOCKING, 0, &err); //�����ź���
    return SDO_Write_Back(id, index, &RxMessage);
}


/**************************************************************************
�������ܣ����SDO���÷���
��ڲ�����ID��������CAN_RX
����  ֵ���ɹ���1
		��ʧ�ܣ�0
��ע�����COB�������֣�������
**************************************************************************/
u8 SDO_Write_Back(u16 id, u16 index, CanRxMsg *rxmsg)
{

    u16 rxindex = (rxmsg->Data[1]) | ((rxmsg->Data[2]) << 8);
    return(((rxmsg->StdId & 0xFF0) == SDO_READ) && (rxmsg->Data[0] == Set_Feedback) && (index == rxindex));
}


/**************************************************************************
��������: SDO������
��ڲ�����ID������
����  ֵ����Ҫ��ȡ������
��ע��
**************************************************************************/
int32_t SDO_Read(u16 id, u16 index)
{
    OS_ERR err;
    u8 msg[8] = {0};
    id = SDO_WRITE + id;
    msg[0] = Select_Len(index, SDO_READ);
    msg[1] = index & 0xFF;
    msg[2] = (index >> 8) & 0xFF;
    msg[3] = NULL;
    while(CAN1_Send_Msg(id, msg, 8));
    OSSemPend(&CAN_SEM, CAN_WaitTime, OS_OPT_PEND_BLOCKING, 0, &err); //�����ź���
    return SDO_Write_Back(id, index, &RxMessage);
}


/**************************************************************************
��������: SDO������
��ڲ�����ID������
����  ֵ����ȡ��ֵ
��ע���ж������������֣����������ֳ��ȶ�ȡ����
**************************************************************************/
int32_t SDO_Read_Back(u16 id, u16 index, CanRxMsg *rxmsg)
{
    int32_t temp;
    u16 rxindex = (rxmsg->Data[1]) | ((rxmsg->Data[2]) << 8);
    if((rxindex == index) && (rxmsg->Data[0] == Select_Len(index, SDO_READ)))
    {
        switch(rxmsg->Data[0])
        {
        case Back_One:
            memcpy(&temp, rxmsg->Data + 4, 1);
            break;
        case Back_Two:
            memcpy(&temp, rxmsg->Data + 4, 2);
            break;
        case Back_Three:
            memcpy(&temp, rxmsg->Data + 4, 3);
            break;
        case Back_Four:
            memcpy(&temp, rxmsg->Data + 4, 4);
            break;
        }
    }
    else return 0;
    return temp;
}


/**************************************************************************
��������: PDO��ȡ���������ٶ�
��ڲ�����CAN_RX, �����Ϣ
����  ֵ��ID��Ӧ��1
		��ID������0
��ע
**************************************************************************/
u8 PDO_Back(CanRxMsg *can, ServoMotor *motor)
{

    if(motor->ID == (can->StdId & 0x00F))
        return 0;
    else
    {
        if((can->StdId & 0xFF0) == TPDO2)
        {
            memcpy(&motor->odom, can->Data + 2, 4);
        }
        else if((can->StdId & 0xFF0) == TPDO3)
            memcpy(&motor->actV, can->Data + 2, 2);
    }
    return 1;
}


/**************************************************************************
��������: �ŷ�������ʹ��
��ڲ�����NMTģʽ
����  ֵ���ɹ���6
		��ʧ�ܣ�<6
��ע��
**************************************************************************/
u8 Servo_Init(u8 mode)
{
    u8 status = 0;
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    Servo_Mode(All_Node,  mode);
    if(SDO_Write(Lift_Wheel, ControlWord, ShutDown))					status++;
    if(SDO_Write(Right_Wheel, ControlWord, ShutDown))					status++;
    if(SDO_Write(Lift_Wheel, ControlWord, Swith_On))					status++;
    if(SDO_Write(Right_Wheel, ControlWord, Swith_On))					status++;
    if(SDO_Write(Lift_Wheel, ControlWord, HALT << 8 | Enable_Operate))	status++;
    if(SDO_Write(Right_Wheel, ControlWord, HALT << 8 | Enable_Operate))	status++;
    CPU_CRITICAL_EXIT();
    return status;
}


/**************************************************************************
��������: �ж������ֳ���
��ڲ�������������orд
����  ֵ���ɹ���������
		  ʧ�ܣ�0
��ע��
**************************************************************************/
u8 Select_Len(u16 index, u16 rw)
{
    u8 len = 0;
    switch(index)
    {
    case ControlWord:
        len = 16;
        break;
    case StatusWord:
        len = 16;
        break;
    case Modes_Of_Operation:
        len = 8;
        break;
    case Position_Value_Calaculated:
        len = 32;
        break;
    case Velocity_Value_Calaculated:
        len = 32;
        break;
    case Current_Actual_Value:
        len = 16;
        break;
    case Profile_P2P_Velocity:
        len = 32;
        break;
    case Profile_Acceleration:
        len = 32;
        break;
    case Profile_Deceleration:
        len = 32;
        break;
    case Profile_Velocity:
        len = 32;
        break;
    }
    if (rw == SDO_WRITE)

        switch (len)
        {
        case 8:
            return Set_One;
        case 16:
            return Set_Two;
        case 24:
            return Set_Three;
        case 32:
            return Set_Four;
        }
    else
        switch (len)
        {
        case 8:
            return Back_One;
        case 16:
            return Back_Two;
        case 24:
            return Back_Three;
        case 32:
            return Back_Four;
        }
    return 0;
}





