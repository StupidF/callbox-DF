#include "system.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "iwdg.h"
#include "WirelessControlMain.h"
#include "button_main.h"
#include "wirelessuartdata.h"
#include "MessageMail.h"

u8 k1=0;
u8 k2=0;
u8 k3=0;
u8 k4=0;

/*�ⲿ��������*/
extern u16 task_delay[MAX_TASK];
extern void(*task[])();//��������PCָ��

int  main(void)
{
	u8 i;
//	u16 Tim1_Num_Main = 0;
//	u16 Tim1_Num_Main_Old = 0;
	
	delay_init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //���� NVIC �жϷ��� 2
	
	TIMER3_Init();			//��ʱ��3��ʼ��
	TIMER4_Init();			//��ʱ��4��ʼ��

	IWDG_Init(4,625);    	//���Ź���ʼ��  ���ʱ��Ϊ1s
	InitButtonMain();		//��ʼ��������������������
	InitButtonFunction();	//ɨ��վ��
	InitWirelessMain(19200); //��ʼ��Wirelessģ��
	while(1)
	{
		for(i = 0; i < MAX_TASK; i++)
		{
			if(task_delay[i] == 0)
			{
				run(task[i]); 
				break;
			}
		}
	}
}




