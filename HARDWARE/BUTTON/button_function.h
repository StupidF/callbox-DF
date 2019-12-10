/**
  ******************************************************************************
  * 文件名  : button_function.h
  * 描述    : Button_Board工程按钮指示灯库函数头文件
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
  ******************************************************************************
  */

#ifndef __BUTTON_FUNCTION_H
#define __BUTTON_FUNCTION_H

/* 包含系统头文件 */
#include "stm32f10x.h"

/* 包含自定义头文件 */
#include "key.h"
extern unsigned char CURRENTSTATION;

/*******************************************************************************
* 名称: InitButtonFunction
* 功能: InitButtonFunction初始化操作
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void InitButtonFunction(void);
/*******************************************************************************
* 名称: SetTaskRun,SetTaskStop
* 功能: SetTaskRun,SetTaskStop 开始 停止任务
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void SetTaskRun(TASKNO taskNo,BUTTONPOS pos); //设置执行任务
void SetTaskStop(TASKNO taskNo);//设置停止任务

void SetCallAGVToState2(void); //设置站点呼叫AGV的阶段为2阶段
/*******************************************************************************
* 名称: TaskCallAGV
* 功能: TaskCallAGV 任务详细
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void TaskCallAGV(void); //呼叫AGV
void TaskGiveWayAGV(void);//放行AGV
void TaskSetBackAGV(void); //命令AGV返回原点
/*******************************************************************************
* 名称: TaskRun
* 功能: TaskCallAGV 任务详细
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void TaskRun(TASKNO taskNo);
/*******************************************************************************
* 名称: ButtonFunctionMain
* 功能: ButtonFunctionMain按钮功能主函数
* 形参: 无
* 返回: 无
* 说明: 20ms执行一次
******************************************************************************/
void ButtonFunctionMain(void);

//充电指令
void SetAction_CMD(Action act_cmd);
//清除指令
void ClearAction_CMD(void );
//充电动作执行
void Action_CMD(void);
Action GetAction(void);
void SetAction_Replay(Action act_cmd);
void ClearAction_Replay(Action act_cmd);
#endif
