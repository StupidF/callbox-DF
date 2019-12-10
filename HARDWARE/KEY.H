/**
  ******************************************************************************
  * 文件名  : button.h 
  * 描述    : Button_Board工程按钮库函数头文件
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
  ******************************************************************************
  */
#ifndef __BUTTON_H
#define __BUTTON_H

/* 包含系统头文件 */
#include "stm32f10x.h"

/*按钮端口定义*/
//#define Button_PIN_1       GPIOC,GPIO_PIN_1
//#define Button_PIN_2       GPIOC,GPIO_PIN_2
//#define Button_PIN_3       GPIOG,GPIO_PIN_2
//#define Button_PIN_4       GPIOG,GPIO_PIN_3
//#define Button_PIN_5       GPIOG,GPIO_PIN_4
//#define Button_PIN_6       GPIOG,GPIO_PIN_5
//#define Button_PIN_7       GPIOG,GPIO_PIN_6
//#define Button_STATIONSCAN GPIOF,GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7

//声明按键动作结构体
typedef enum _BUTTONSTATE
{
	BUTTON_OFF       = (u8)0, //按钮松开中
	BUTTON_OFF_TO_ON = (u8)1, //按钮按下边缘
	BUTTON_ON        = (u8)2, //按钮按下中
	BUTTON_ON_TO_OFF = (u8)3  //按钮松开边缘
}BUTTONSTATE;

//声明按键序号
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

/*任务状态*/
typedef enum _TASKNO_
{
	Task_NULL             = (u8)0,	//无任务
	Task_CallAGV          = (u8)1,	//呼叫AGV
	Task_GiveWayAGV       = (u8)2,	//放行AGV
	Task_SetBackAGV       = (u8)3,	//设置AGV返回原点
	Task_AskAGV           = (u8)4,	//呼叫AGV
	Task_CallStartNowAGV  = (u8)5,	//呼叫启动当前站点AGV
	Task_StopCallAGV      = (u8)6,	//呼叫启动当前站点AGV
	Task_ChargePower      = (u8)7,	//呼叫启动当前站点AGV
	Task_CallAGVK         = (u8)8,
	Task_BeginCharge      = (u8)9,	//开始充电
	Task_StopCharge       = (u8)10	//停止充电
}TASKNO;

typedef enum _Action
{
	Action_NULL      = (u8)0,
	Begin_Charge     = (u8)1,
	Stop_Charge      = (u8)2,
	BeginZuDang      = (u8)3,
	Stop_Zudang      = (u8)4,
}Action;


/*函数声明*/

/*******************************************************************************
* 名称: Button_Init 按键端口配置
* 功能: 开机配置按键端口
* 形参: 无
* 返回: 当前站点
* 说明: 无
******************************************************************************/
void Button_Init(void);
/*******************************************************************************
* 名称: ScandCurrentStation 扫描当前站点
* 功能: 开机扫描当前站点
* 形参: 无
* 返回: 当前站点
* 说明: 无
******************************************************************************/
unsigned char ScandCurrentStation(void);
/*******************************************************************************
* 名称: KeyScan
* 功能: 按键循环扫描函数
* 形参: 无
* 返回: 无
* 说明: 无
********************************************************************************/
void ButtonScan(void);

/*******************************************************************************
* 名称: GetButtonState
* 功能: 获取当前按键信息
* 形参: buttonPos 按键位置
* 返回: 无
* 说明: 无
********************************************************************************/
BUTTONSTATE GetButtonState(u8 buttonPos);

/*******************************************************************************
* 名称: Button_Init
* 功能: 配置按键端口
* 形参: 无
* 返回: 无
* 说明: 无
********************************************************************************/



#endif
