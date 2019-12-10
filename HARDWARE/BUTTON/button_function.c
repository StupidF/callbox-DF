/***********************************************
  * 文件名  : button_led.c 
  * 描述    : Button_Board工程按钮指示灯库函数
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
************************************************/
/* 包含自定义头文件 */
#include "button_function.h"
#include "led.h"
#include "key.h"
#include "MessageMail.h"
#include "sys.h"

#define MSGREPLYTIMES   8

#define DESWEBID        0xFFFF

/*函数定义*/
u8 FuctionTaskFlag[17];
u8 OldFuctionTaskFlag[17];
u8 TaskButtonPos[17];    

unsigned char CURRENTSTATION=0; //站点信息初始化为0
static u8 CallAGVState=0;	 	//站点呼叫AGV的阶段
static u8 CallAGVEndDelay=0; 	//站点呼叫小车的次数延时
MSGSTRUCT TaskMsgSend;			//结构体变量 发送数据..

static u16 Timertick = 0;
Action Act_cmd;					//充电动作.Act_cmd
Action Act_cmdold;

unsigned char Act_step=0;		//充电过程
unsigned int  Act_tick=0;		//充电过程


/**************************************************************
* 名称: InitButtonFunction
* 功能: InitButtonFunction初始化操作
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
***************************************************************/
void InitButtonFunction(void)
{
	u8 i;
	SetOnLED(1);
	SetOnLED(2);
//	SetOnLED(3);
//	CURRENTSTATION=ScandCurrentStation();     //扫描站点设置     灯亮到灭  扫描完成
	CURRENTSTATION=38;
	SetOffLED(1);
	SetOffLED(2);
//	SetOffLED(3);
	for(i=0;i<17;i++)			//初始化标志
	{
		FuctionTaskFlag[i]=0;
		OldFuctionTaskFlag[i]=0;
	}
}

/*******************************************************************************
* 名称: SetTaskRun,SetTaskStop
* 功能: SetTaskRun,SetTaskStop 开始 停止任务
* 形参: 小车任务过程     
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void SetTaskRun(TASKNO taskNo,BUTTONPOS pos) 	//设置执行任务
{
	if(taskNo==Task_CallAGV)					//呼叫小车
	{
		if(FuctionTaskFlag[taskNo]==0)
		{
			FuctionTaskFlag[taskNo]=1;
			CallAGVState=1;
		}
		else 
		{
			FuctionTaskFlag[taskNo]=0;
			CallAGVState=0;
		}
		TaskButtonPos[taskNo]=pos;
	}
	else if(taskNo == Task_CallStartNowAGV)
	{
		if(FuctionTaskFlag[Task_CallStartNowAGV] == 0)
			FuctionTaskFlag[Task_CallStartNowAGV] = 1;
		else
			FuctionTaskFlag[Task_CallStartNowAGV] = 0;
		TaskButtonPos[taskNo]=pos;
	}
	else if(taskNo == Task_CallStartNowAGV)
	{
	
	}
	else		// if(taskNo<17)
	{
		FuctionTaskFlag[taskNo]=1;
		TaskButtonPos[taskNo]=pos;
	}
}


void SetTaskStop(TASKNO taskNo)//设置停止任务
{
	if(taskNo<7)
		FuctionTaskFlag[taskNo]=0;
}


void SetCallAGVToState2(void) //设置站点呼叫AGV的阶段为2阶段
{
	CallAGVState=3;
	CallAGVEndDelay=4;
}

/*
void SetReverceTask(TASKNO taskNo)//设置任务状态反转
{
	if(taskNo<17)
	{
		if(FuctionTaskFlag[taskNo]==0)
			FuctionTaskFlag[taskNo]=1;
		else
			FuctionTaskFlag[taskNo]=0;
	}
}*/

/*****************************************************************************
* 名称: TaskCallAGV
* 功能: TaskCallAGV 任务详细
* 形参: 无
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void TaskCallAGV(void) 		//呼叫AGV
{
	/* 自定义新类型 
	typedef struct _MSGSTRUCT
	{
		u8 CmdNo   ; //接收帧命令
		u8 Data[9] ; //接收帧数据
		u16 WebID  ; //接收帧网络
	}MSGSTRUCT;*/
	static u16 TaskDelay1=0;
	if(0/*FuctionTaskFlag[Task_CallAGV]==1*/)
	{
		TaskDelay1+=1;
		if(CallAGVState==1)    //呼叫小车阶段1
		{
			if(TaskDelay1<25)
				SetOnLED(TaskButtonPos[Task_CallAGV]);
			else if(TaskDelay1<50)
			{
				if(TaskDelay1==25)     //将数据写入待发送数组
				{
					TaskMsgSend.CmdNo=0x72;
					TaskMsgSend.Data[0]=CURRENTSTATION;
					TaskMsgSend.Data[1]=0x01;
					TaskMsgSend.Data[2]=0x04;
					TaskMsgSend.Data[3]=CURRENTSTATION;
					TaskMsgSend.WebID=DESWEBID;
					AddSendMessage(TaskMsgSend);			
				}
				SetOffLED(TaskButtonPos[Task_CallAGV]);
			}
			else
			{
				TaskDelay1  = 0;
				TaskMsgSend.CmdNo   = 0x72;
				TaskMsgSend.Data[0] = CURRENTSTATION;
				TaskMsgSend.Data[1] = 0x01;
				TaskMsgSend.Data[2] = 0x04;
				TaskMsgSend.Data[3] = CURRENTSTATION;
				TaskMsgSend.WebID  = DESWEBID;
				TaskMsgSend.WebID  = DESWEBID;
				AddSendMessage(TaskMsgSend);
			}
		}
		else if(CallAGVState == 2)     //呼叫小车阶段2
		{
			SetOnLED(TaskButtonPos[Task_CallAGV]);
			if(TaskDelay1>=25)
			{
				TaskMsgSend.CmdNo  = 0x73;
				TaskMsgSend.Data[0]=CURRENTSTATION;
				TaskMsgSend.Data[1]=0x01;
				TaskMsgSend.Data[2]=0x05;
				TaskMsgSend.Data[3]=CURRENTSTATION;
				TaskMsgSend.WebID=DESWEBID;
				TaskMsgSend.WebID=DESWEBID;
				AddSendMessage(TaskMsgSend);
				TaskDelay1=0;
				if(CallAGVEndDelay>=1)
					CallAGVEndDelay-=1;
				else
					FuctionTaskFlag[Task_CallAGV]=0;
			}
		}
		else
			FuctionTaskFlag[Task_CallAGV] = 0;		//完成后清零
	}
	else
		SetOffLED(TaskButtonPos[Task_CallAGV]);
}

void TaskCallAGVK(void) 		//呼叫AGV
{
	/* 自定义新类型 
	typedef struct _MSGSTRUCT
	{
		u8 CmdNo   ; //接收帧命令
		u8 Data[9] ; //接收帧数据
		u16 WebID  ; //接收帧网络
	}MSGSTRUCT;*/
	static u16 TaskDelay1=0;
	if(FuctionTaskFlag[Task_CallAGVK]==1)
	{
		TaskDelay1+=1;
		if(CallAGVState==1)    //呼叫小车阶段1
		{
			if(TaskDelay1<25)
				SetOnLED(TaskButtonPos[Task_CallAGVK]);
			else if(TaskDelay1<50)
			{
				if(TaskDelay1==25)     //将数据写入待发送数组
				{
					TaskMsgSend.CmdNo=0x73;
					TaskMsgSend.Data[0]=CURRENTSTATION;
					TaskMsgSend.Data[1]=0x01;
					TaskMsgSend.Data[2]=0x04;
					TaskMsgSend.Data[3]=CURRENTSTATION;
					TaskMsgSend.WebID=DESWEBID;
					AddSendMessage(TaskMsgSend);			
				}
				SetOffLED(TaskButtonPos[Task_CallAGV]);
			}
			else
			{
				TaskDelay1  = 0;
				TaskMsgSend.CmdNo   = 0x73;
				TaskMsgSend.Data[0] = CURRENTSTATION;
				TaskMsgSend.Data[1] = 0x01;
				TaskMsgSend.Data[2] = 0x04;
				TaskMsgSend.Data[3] = CURRENTSTATION;
				TaskMsgSend.WebID  = DESWEBID;
				TaskMsgSend.WebID  = DESWEBID;
				AddSendMessage(TaskMsgSend);
			}
		}
		else if(CallAGVState == 2)     //呼叫小车阶段2
		{
			SetOnLED(TaskButtonPos[Task_CallAGVK]);
			if(TaskDelay1>=25)
			{
				TaskMsgSend.CmdNo  = 0x73;
				TaskMsgSend.Data[0]=CURRENTSTATION;
				TaskMsgSend.Data[1]=0x01;
				TaskMsgSend.Data[2]=0x05;
				TaskMsgSend.Data[3]=CURRENTSTATION;
				TaskMsgSend.WebID=DESWEBID;
				TaskMsgSend.WebID=DESWEBID;
				AddSendMessage(TaskMsgSend);
				TaskDelay1=0;
				if(CallAGVEndDelay>=1)
					CallAGVEndDelay-=1;
				else
					FuctionTaskFlag[Task_CallAGVK]=0;
			}
		}
		else
			FuctionTaskFlag[Task_CallAGVK] = 0;		//完成后清零
	}
	else
		SetOffLED(TaskButtonPos[Task_CallAGVK]);
}

void TaskStopCallAGV(void)			//停止呼叫AGV小车
{
	if(FuctionTaskFlag[Task_StopCallAGV]==1)
	{
		SetTaskStop(Task_CallAGV);
		SetTaskStop(Task_StopCallAGV);
	}
}

void TaskGiveWayAGV(void)//放行AGV
{
	static u8 TaskDelay=0,TaskTimes=0;
	if(TaskDelay<200)
		TaskDelay++;
	if(0/*FuctionTaskFlag[Task_GiveWayAGV]==1*/)
	{
		if(OldFuctionTaskFlag[Task_GiveWayAGV]==0)
				TaskTimes=MSGREPLYTIMES;
		if(TaskDelay>20/*最短0.4S执行一次*/)
		{
			TaskDelay=0;
			TaskMsgSend.CmdNo=0x77;
			TaskMsgSend.Data[0]=CURRENTSTATION;
			TaskMsgSend.WebID=DESWEBID;
			AddSendMessage(TaskMsgSend);			
			if(TaskTimes>=1)
				TaskTimes-=1;
		}
		if(TaskTimes==0)
			FuctionTaskFlag[Task_GiveWayAGV]=0;
	}
	OldFuctionTaskFlag[Task_GiveWayAGV]=FuctionTaskFlag[Task_GiveWayAGV];
}

void TaskSetBackAGV(void) //命令AGV返回原点
{
	static u8 TaskDelay=0,TaskTimes=0;
	if(TaskDelay < 200)
		TaskDelay++;
	if(0/*FuctionTaskFlag[Task_SetBackAGV]==1*/)
	{
		if(OldFuctionTaskFlag[Task_SetBackAGV]==0)
				TaskTimes = MSGREPLYTIMES;
	    if(TaskDelay>20 /*最短0.4S执行一次*/)
		{
			TaskDelay = 0;
			TaskMsgSend.CmdNo   = 0x78;      
			TaskMsgSend.Data[0] = CURRENTSTATION;
			TaskMsgSend.WebID   = DESWEBID;
			AddSendMessage(TaskMsgSend);
			
			if(TaskTimes>=1)
				TaskTimes-=1;
		}
		if(TaskTimes==0)
			FuctionTaskFlag[Task_SetBackAGV]=0;
	}
	OldFuctionTaskFlag[Task_SetBackAGV]=FuctionTaskFlag[Task_SetBackAGV];
}

void TaskCallStartNowAGV(void) //呼叫启动当前站点AGV
{
	static u8 TaskTimes=0,LedState=0;
	if(0/*FuctionTaskFlag[Task_CallStartNowAGV]==1*/)
	{
		
		TaskTimes+=1;
		if(TaskTimes>=40)
		{
			TaskTimes=0;
			//发送启动命令
			TaskMsgSend.CmdNo   = 0x71;
			TaskMsgSend.Data[0] = CURRENTSTATION;
			TaskMsgSend.Data[1] = 0x02;//发送启动命令
			TaskMsgSend.Data[2] = 0x01;
			TaskMsgSend.Data[3] = 0;
			TaskMsgSend.Data[4] = 0;
			TaskMsgSend.Data[5] = 0;
			TaskMsgSend.Data[6] = 0;
			TaskMsgSend.Data[7] = 0;
			TaskMsgSend.Data[8] = 0;
			TaskMsgSend.WebID   = DESWEBID;
			AddSendMessage(TaskMsgSend);
			if(LedState==0)
			{
				SetOnLED(TaskButtonPos[Task_CallStartNowAGV]);
				LedState=1;
			}
			else
			{
				SetOffLED(TaskButtonPos[Task_CallStartNowAGV]);
				LedState = 0;
			}
		}
	}
	else
	{
		SetOffLED(TaskButtonPos[Task_CallStartNowAGV]);
	}
}
/*******************************************************************************
* 名称: TaskRun
* 功能: TaskCallAGV 任务详细
* 形参: TaskNo 任务标识
* 返回: 无
* 说明: 强推挽高速输出低电平 
******************************************************************************/
void TaskRun(TASKNO taskNo)
{
	switch(taskNo)
	{
		case Task_CallAGV:
			 TaskCallAGV();
			break;
		case Task_GiveWayAGV:
			 TaskGiveWayAGV();
			break;
		case Task_SetBackAGV:
			 TaskSetBackAGV();
			break;
		case Task_CallStartNowAGV:
			 TaskCallStartNowAGV();
			break;
		case Task_StopCallAGV:
			 TaskStopCallAGV();
			break;
		case  Task_CallAGVK:
			TaskCallAGVK();
			break;
//		case Task_BeginCharge:
//			break;
		default:break;
	}
}


void OUTPUll(void)
{
	HardwareLightONLed(3);	//shineng 
	HardwareLightONLed(4);
}
void INPULL(void)
{
	HardwareLightONLed(3);	//shineng 
	HardwareLightOFFLed(4);
}
void KEEP(void)
{
	HardwareLightOFFLed(3);
	HardwareLightOFFLed(4);
}
void OLELLY_ENABLE(void)
{
	HardwareLightONLed(3);
}
//使能阻挡	//第三通道“L”引脚
void Out(void)
{
	HardwareLightONLed(3);
}
//失能阻挡   //第三通道“L”引脚
void DisOut(void)
{
	HardwareLightOFFLed(3);
}



/*******************************************************************************
* 名称: ButtonFunctionMain
* 功能: ButtonFunctionMain按钮功能主函数
* 形参: 无
* 返回: 无
* 说明: 20ms执行一次
******************************************************************************/
void ButtonFunctionMain(void)
{
	u8 i;
	for(i=0;i<7;i++)
		TaskRun((TASKNO)i);
}

void SetAction_CMD(Action act_cmd)
{
	Act_cmd = act_cmd;
}
void ClearAction_CMD(void )
{	
	Act_cmd = Action_NULL;
}

Action GetAction()
{
	return Act_cmd;
}

unsigned char Begin_ChargeReplay = 0;
unsigned char Stop_ChargeReplay  = 0;

unsigned char BeginZuDangReplay = 0;
unsigned char Stop_ZudangReplay  = 0;

void SetAction_Replay(Action act_cmd)
{
	switch(act_cmd)
	{
		case Begin_Charge:
			Begin_ChargeReplay = 1;
			break;
		case Stop_Charge:
			Stop_ChargeReplay = 1;
			break;
		case BeginZuDang:
			BeginZuDangReplay = 1;
			break;
		case Stop_Zudang:
			Stop_ZudangReplay = 1;
			break;
		default:
			break;
	
	}
	Act_cmd = act_cmd;
}


void ClearAction_Replay(Action act_cmd)
{
	switch(act_cmd)
	{
		case Begin_Charge:
			Begin_ChargeReplay = 0;
			break;
		case Stop_Charge:
			Stop_ChargeReplay = 0;
			break;
		default:
			break;
	}
	Act_cmd = act_cmd;
}

void Action_CMD(void)
{
	static unsigned char ReplyFlag=0;
	if(Act_cmd != Act_cmdold)
	{
		Act_step=0;
	}
	if(GetButtonState(4) == BUTTON_ON)	//有信号 清楚计时
	{
		Timertick = 0;
	}
	else		//无信号  开始计时
	{
		Timertick++;
		if(Timertick>500)		//500时间需要测定
		{	
			Act_cmd = Action_NULL;
			if(Timertick>600 && GetButtonState(2) == BUTTON_ON)	//600时间需要实际测试			2通T引脚收回光电信号
			{
				SendACall_CAR();
				Timertick=500;
			}
			DisOut();
		}
	}
	switch(Act_cmd)
	{
		case Action_NULL:
		//	KEEP();
//			if(GetButtonState(1) == BUTTON_ON)
//			{
//				Act_step = 0;
//			}
//			else 
//			{
//				Act_step = 0;
//			}
//			if(GetButtonState(2) == BUTTON_ON)
//			{
//				Act_step = 0;
//			}
//			else 
//			{
//				Act_step = 0;
//			}
//			if(GetButtonState(3) == BUTTON_ON)
//			{
//				Act_step = 0;
//			}
//			else 
//			{
//				Act_step = 0;
//			}
			
			
			Act_step = 0;
			break;
		case Begin_Charge:
			if(Act_step == 0)
			{
				if(GetButtonState(3) != BUTTON_ON || GetButtonState(2) == BUTTON_ON )	//光电传感器没有信号
				{
					OUTPUll();
					Act_step = 1;
				}
			}
			else if(Act_step == 1)
			{
				if(GetButtonState(3) == BUTTON_ON  || GetButtonState(2) == BUTTON_ON )		
				{
					KEEP();
					TaskMsgSend.CmdNo   = 0x22;
					if(Begin_ChargeReplay == 1)
					{
						Begin_ChargeReplay = 0;
						ReplyFlag = 1;
					}
					
				}
				else 
				{
					OUTPUll();
				}
			}
			break;
		case Stop_Charge:
			if(Act_step == 0)
			{
				if(GetButtonState(1) != BUTTON_ON )	//光电传感器没有信号
				{
					INPULL();
					Act_step = 1;
				}
			}
			else if(Act_step == 1)
			{
				if(GetButtonState(1) == BUTTON_ON )
				{
					KEEP();
					TaskMsgSend.CmdNo   = 0x24;
					if(Stop_ChargeReplay == 1)
					{
						Stop_ChargeReplay = 0;
						ReplyFlag = 1;
					}
				}
				else 
				{
					INPULL();
				}
			}
		case BeginZuDang:
			if(Act_step == 0)
			{
				Act_step = 1;
			}
			if(Act_step == 1)
			{
				Out();
				if(GetButtonState(3) == BUTTON_ON )
				{
					TaskMsgSend.CmdNo   = 0x41;
					if(BeginZuDangReplay == 1)
					{
						BeginZuDangReplay = 0;
						ReplyFlag = 1;
					}
				}
			}
			break;
		case Stop_Zudang:
			if(Act_step == 0)
			{
				Act_step =1;
			}
			if(Act_step == 1)
			{
				DisOut();
				if(GetButtonState(2) == BUTTON_ON )
				{
					TaskMsgSend.CmdNo   = 0x41;
					if(Stop_ZudangReplay == 1)
					{
						Stop_ZudangReplay = 0;
						ReplyFlag = 1;
					}
				}
			}
			break;
		default:
			break;
	}
	if(ReplyFlag == 1)
	{
		ReplyFlag= 0;
		AddSendMessage(TaskMsgSend);
	}
		
		
	Act_cmdold = Act_cmd;
}
