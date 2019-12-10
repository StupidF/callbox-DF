/**
  ******************************************************************************
  * �ļ���  : button_function.c 
  * ����    : Button_Board���̰�ťִ�к�����
  * ��汾  : V2.1.0
  * ����    :���
  * �޸�ʱ��:2013-8-24
  ******************************************************************************
  */

/* �����Զ���ͷ�ļ� */
#include "button_main.h"
#include "led.h"
#include "key.h"

BUTTON_STRUCT MyButton[16]={
							{BUTTONNULL,Task_NULL},  		//1
							{BUTTONNULL,Task_NULL},			//2
							{BUTTONNULL,Task_NULL}, 		//3
							{BUTTONNULL,Task_NULL}, 		//4
							{BUTTONNULL,Task_NULL}, 		//5
							{BUTTONNULL,Task_NULL}, 		//6
							{BUTTONNULL,Task_NULL}, 		//7
							{BUTTONNULL,Task_NULL}, 		//8
							{BUTTONNULL,Task_NULL}, 		//9
							{BUTTONNULL,Task_NULL}, 		//10
							{BUTTONNULL,Task_NULL}, 		//11
							{BUTTONNULL,Task_NULL}, 		//12
							{BUTTONNULL,Task_NULL}, 		//10
							{BUTTONNULL,Task_NULL}, 		//13
							{BUTTONNULL,Task_NULL}, 		//14
							{BUTTONNULL,Task_NULL}, 		//15
							};

/*******************************************************************************
* ����: InitButtonMain
* ����: InitButtonMain��ʼ��
* �β�: ��
* ����: ��
* ˵��: ǿ�����������͵�ƽ 
******************************************************************************/
void InitButtonMain(void)
{
	Button_Init();				//��ť��ʼ��
	Button_LED_Init();			//led�Ƶĳ�ʼ��
}

/*******************************************************************************
* ����: InitButtonMain
* ����: InitButtonMain��ť����������
* �β�: ��
* ����: ��
* ˵��: 20msִ��һ��
******************************************************************************/
void ButtonMain(void)
{
	//��������˳���ܷ�ButtonScan();ButtonFunctionMain();ButtonLED_Show();
	u8 i;
	ButtonScan();
	for(i=0;i<16;i++)
	{
		if(MyButton[i].pos!=BUTTONNULL)
		{
	//		j=MyButton[i].taskNo;
			if(GetButtonState(MyButton[i].pos)==BUTTON_ON_TO_OFF)
			{
				SetTaskRun(MyButton[i].taskNo,MyButton[i].pos);
			}
		}
	}
	ButtonFunctionMain();
	ButtonLED_Show();
	if(GetButtonState(4)== BUTTON_ON)
	{
		SetAction_CMD(Begin_Charge);
	}
	else if(GetButtonState(4)== BUTTON_ON_TO_OFF)
	{
		SetAction_CMD(Stop_Charge);
	}
	Action_CMD();
}











