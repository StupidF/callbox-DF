/**
  ******************************************************************************
  * �ļ���  : button.h 
  * ����    : Button_Board���̰�ť�⺯��ͷ�ļ�
  * ��汾  : V2.1.0
  * ����    :���
  * �޸�ʱ��:2013-8-24
  ******************************************************************************
  */
#ifndef __BUTTON_H
#define __BUTTON_H

/* ����ϵͳͷ�ļ� */
#include "stm32f10x.h"

/*��ť�˿ڶ���*/
//#define Button_PIN_1       GPIOC,GPIO_PIN_1
//#define Button_PIN_2       GPIOC,GPIO_PIN_2
//#define Button_PIN_3       GPIOG,GPIO_PIN_2
//#define Button_PIN_4       GPIOG,GPIO_PIN_3
//#define Button_PIN_5       GPIOG,GPIO_PIN_4
//#define Button_PIN_6       GPIOG,GPIO_PIN_5
//#define Button_PIN_7       GPIOG,GPIO_PIN_6
//#define Button_STATIONSCAN GPIOF,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

//�������������ṹ��
typedef enum _BUTTONSTATE
{
	BUTTON_OFF       = (u8)0, //��ť�ɿ���
	BUTTON_OFF_TO_ON = (u8)1, //��ť���±�Ե
	BUTTON_ON        = (u8)2, //��ť������
	BUTTON_ON_TO_OFF = (u8)3  //��ť�ɿ���Ե
}BUTTONSTATE;

//�����������
typedef enum _BUTTONPOS
{
	BUTTONNULL = (u8)0,
	BUTTON1    = (u8)1,
	BUTTON2    = (u8)2,
	BUTTON3    = (u8)3,
	BUTTON4    = (u8)4,
	BUTTON5    = (u8)5,
	BUTTON6    = (u8)6,
	BUTTON7    = (u8)7,
	BUTTON8    = (u8)8,
	BUTTON9    = (u8)9,
	BUTTON10   = (u8)10,
	BUTTON11   = (u8)11,
	BUTTON12   = (u8)12,
	BUTTON13   = (u8)13,
	BUTTON14   =( u8)14,
	BUTTON15   =( u8)15,
	BUTTON16   =( u8)16
}BUTTONPOS;

/*����״̬*/
typedef enum _TASKNO_
{
	Task_NULL             = (u8)0,	//������
	Task_CallAGV          = (u8)1,	//����AGV
	Task_GiveWayAGV       = (u8)2,	//����AGV
	Task_SetBackAGV       = (u8)3,	//����AGV����ԭ��
	Task_AskAGV           = (u8)4,	//����AGV
	Task_CallStartNowAGV  = (u8)5,	//����������ǰվ��AGV
	Task_StopCallAGV      = (u8)6,	//����������ǰվ��AGV
	Task_ChargePower      = (u8)7,	//����������ǰվ��AGV
	Task_CallAGVK         = (u8)8,
	Task_BeginCharge      = (u8)9,	//��ʼ���
	Task_StopCharge       = (u8)10	//ֹͣ���
}TASKNO;

typedef enum _Action
{
	Action_NULL      = (u8)0,
	Begin_Charge     = (u8)1,
	Stop_Charge      = (u8)2,
	BeginZuDang      = (u8)3,
	Stop_Zudang      = (u8)4,
}Action;


/*��������*/

/*******************************************************************************
* ����: Button_Init �����˿�����
* ����: �������ð����˿�
* �β�: ��
* ����: ��ǰվ��
* ˵��: ��
******************************************************************************/
void Button_Init(void);
/*******************************************************************************
* ����: ScandCurrentStation ɨ�赱ǰվ��
* ����: ����ɨ�赱ǰվ��
* �β�: ��
* ����: ��ǰվ��
* ˵��: ��
******************************************************************************/
unsigned char ScandCurrentStation(void);
/*******************************************************************************
* ����: KeyScan
* ����: ����ѭ��ɨ�躯��
* �β�: ��
* ����: ��
* ˵��: ��
********************************************************************************/
void ButtonScan(void);

/*******************************************************************************
* ����: GetButtonState
* ����: ��ȡ��ǰ������Ϣ
* �β�: buttonPos ����λ��
* ����: ��
* ˵��: ��
********************************************************************************/
BUTTONSTATE GetButtonState(u8 buttonPos);

/*******************************************************************************
* ����: Button_Init
* ����: ���ð����˿�
* �β�: ��
* ����: ��
* ˵��: ��
********************************************************************************/



#endif