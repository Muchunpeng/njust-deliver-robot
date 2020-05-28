#include "usbh_hid_gamepad.h"
#include "usart.h"


//FC��Ϸ�ֱ�ģ��
//��ȡfcpad.ctrlval,���ɵõ���ǰ�ֱ���ʽת����FC�ֱ���ʽ.
FC_GamePad_TypeDef fcpad;
FC_GamePad_TypeDef fcpad1;


static void  GAMEPAD_Init (void);
static void  GAMEPAD_Decode(u8	*data);


HID_cb_TypeDef HID_GAMEPAD_cb =
{
    GAMEPAD_Init,
    GAMEPAD_Decode,
};

//game pad ��ʼ��
static void  GAMEPAD_Init ( void)
{
    USR_GAMEPAD_Init();
}

//ͨ��USB���������,��������fcpad����
//data0,data1:USB�õ��ķ�������
//���ݸ�ʽ����:
//data0:00,�������;ff,�Ҽ�����,7F,û�а�������
//data1:00,�ϼ�����;ff,�¼�����,7F,û�а�������
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
//ͨ��USB AB������,��������fcpad����
//data:USB�õ���1/2/3/4������
//���ݸ�ʽ����:
//data:���4λ��Ч
//b4=1,1������(FC:B������)
//b5=1,2������(FC:A������)
//b6=1,3������(FC:A��)
//b7=1,4������(FC:B��)
void GAMEPAD_nAB_Decode(u8 data)
{
    if(data & 0X10)
        fcpad.a = 1;	//A������
    else
        fcpad.a = 0;	//A������
    if(data & 0X20)
        fcpad.b = 1;	//B������
    else
        fcpad.b = 0;	//B������
    if(data & 0X40)
        fcpad.c = 1;	//C������
    else
        fcpad.c = 0;	//C������
    if(data & 0X80)
        fcpad.d = 1;	//D������
    else
        fcpad.d = 0;	//D������
}
//ͨ��USB ���ܼ�����,��������fcpad����
//data:USB�õ���Select/Start������
//���ݸ�ʽ����:
//data:b4,b5��Ч.
//b4=1,Select������
//b5=1,Start������
void GAMEPAD_nFun_Decode(u8 data)
{
    if(data & 0X10)fcpad.select = 1;	//Select������
    else fcpad.select = 0;			//Select���ɿ�
    if(data & 0X20)fcpad.start = 1;	//Start������
    else fcpad.start = 0;			//Start���ɿ�

    if(data & 0X04)fcpad.l1 = 1;	//L1������
    else fcpad.l1 = 0;			//L1���ɿ�
    if(data & 0X01)fcpad.l2 = 1;	//L2������
    else fcpad.l2 = 0;			//L2���ɿ�
    if(data & 0X08)fcpad.r1 = 1;	//R1������
    else fcpad.r1 = 0;			//R1���ɿ�
    if(data & 0X02)fcpad.r2 = 1;	//R2������
    else fcpad.r2 = 0;			//R2���ɿ�
}

//game pad���ݽ���
static void  GAMEPAD_Decode(uint8_t *data)
{
    {
        GAMEPAD_nDir_Decode(data[0], data[1]);	//���뷽���
        GAMEPAD_nAB_Decode(data[5]);			//����AB��
        GAMEPAD_nFun_Decode(data[6]);			//���빦�ܼ�
    }
    if(fcpad.up)printf("��\n");
    if(fcpad.right)printf("��\n");
    if(fcpad.down)printf("��\n");
    if(fcpad.left)printf("��\n");
    if(fcpad.select)printf("ѡ��\n");
    if(fcpad.start)printf("��ʼ\n");
    if(fcpad.a)printf("A\n");
    if(fcpad.b)printf("B\n");
    if(fcpad.c)printf("C\n");
    if(fcpad.d)printf("D\n");
    if(fcpad.l1)printf("L1\n");
    if(fcpad.l2)printf("L2\n");
    if(fcpad.r1)printf("R1\n");
    if(fcpad.r2)printf("R2\n");

}


















