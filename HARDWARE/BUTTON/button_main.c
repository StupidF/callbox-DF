/**
  ******************************************************************************
  * 文件名  : button_function.c 
  * 描述    : Button_Board工程按钮执行函数库
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
  ******************************************************************************
  */

/* 包含自定义头文件 */
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
* 名称: InitButtonMain
* 功能: InitButtonMain初始化
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void InitButtonMain(void)
{
	Button_Init();				//按钮初始化
	Button_LED_Init();			//led灯的初始化
}

/*******************************************************************************
* 名称: InitButtonMain
* 功能: InitButtonMain按钮控制主程序
* 形参: 无
* 返回: 无
* 说明: 20ms执行一次
******************************************************************************/
void ButtonMain(void)
{
	//三条函数顺序不能反ButtonScan();ButtonFunctionMain();ButtonLED_Show();
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











