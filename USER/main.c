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

/*外部变量声明*/
extern u16 task_delay[MAX_TASK];
extern void(*task[])();//获得任务的PC指针

int  main(void)
{
	u8 i;
//	u16 Tim1_Num_Main = 0;
//	u16 Tim1_Num_Main_Old = 0;
	
	delay_init();	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置 NVIC 中断分组 2
	
	TIMER3_Init();			//定时器3初始化
	TIMER4_Init();			//定时器4初始化

	IWDG_Init(4,625);    	//看门狗初始化  溢出时间为1s
	InitButtonMain();		//初始化按键及拨码设置引脚
	InitButtonFunction();	//扫描站点
	InitWirelessMain(19200); //初始化Wireless模块
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




