#include "system.h"
#include "iwdg.h"
#include "WirelessControlMain.h"
#include "button_main.h"

u16 task_delay[MAX_TASK];		//����������ʱ����
void (*task[])() = {task0,task1,task2,task3,task4};	//��������PCָ��
	
/*******************************************************************************
* ����: run
* ����: ָ������ָ�뺯��
* �β�: void(*ptask)()
* ����: ��
* ˵��: ����������β���һ��ָ������ָ�룬��ָ����ָ���
*       ������һ�����β��޷���ֵ�ĺ���
********************************************************************************/
void run(void (*ptask)(void))
{
	(*ptask)();
}

/*******************************************************************************
* ����: task0
* ����: ����0ִ�к���
* �β�: ��
* ����: ��
* ˵��: ����ÿ��10MS����һ�Σ�Ƶ��Ϊ100HZ
********************************************************************************/
void task0(void)
{
	task_delay[0] = TASK_DELAY0;
}

/*******************************************************************************
* ����: task1
* ����: ����1ִ�к���
* �β�: ��
* ����: ��
* ˵��: ����ÿ��20MS����һ�Σ�Ƶ��Ϊ50HZ
********************************************************************************/
void task1(void)
{
	ButtonMain();
	task_delay[1] = TASK_DELAY1;
}

/*******************************************************************************
* ����: task2
* ����: ����2ִ�к���
* �β�: ��
* ����: ��
* ˵��: ����ÿ��40MS����һ�Σ�Ƶ��Ϊ25HZ
********************************************************************************/
void task2(void)
{
//	CAN_Test();//CAN���߲��Ժ���
	WirelessMain();
	task_delay[2] = TASK_DELAY2;
}

/*******************************************************************************
* ����: task3
* ����: ����3ִ�к���
* �β�: ��
* ����: ��
* ˵��: ����ÿ��100MS����һ�Σ�Ƶ��Ϊ10HZ
********************************************************************************/
void task3(void)
{
	/*ι��*/
	IWDG_Feed();
	task_delay[3] = TASK_DELAY3;
}

/*******************************************************************************
* ����: task4
* ����: ����4ִ�к���
* �β�: ��
* ����: ��
* ˵��: ����ÿ��500MS����һ�Σ�Ƶ��Ϊ2HZ
********************************************************************************/
void task4(void)
{
	/*�����ǰ��BOXû�����κ�AGVͨѶ�ɹ�����㲥������ѯ����*/
//	Function_Read_Station_Conditon();
	task_delay[4] = TASK_DELAY4;
}
/*************************************************
* ����: TaskTickMain
* ����: ������ת����
* �β�: ��
* ����: ��
* ˵��: ��
**************************************************/
void TaskTickMain(void)
{
	u8 i;		/*Angle_Function����ϵͳ���Դ���*/
	for(i = 0; i < MAX_TASK; i++)
	{
		if(task_delay[i])
			task_delay[i]--;	//ÿʱ�ӽ��Ķ�������ʱ������1����Ϊ0������ʼ����
	}
}
