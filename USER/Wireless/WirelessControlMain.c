#include "WirelessControlMain.h"
#include "WirelessUartData.h"
#include "MessageMail.h"
#include "key.h"
#include "button_function.h"
#include "led.h"

MSGSTRUCT   MsgRecieve;
MSGSTRUCT   MsgSend;

void ZigbeeTranslateMsg(void)		//处理接收到的数据串
{	
	static unsigned short timeCount=0;
 	static unsigned char ReplyFlag=0,i;
	ReplyFlag=0;
	timeCount++;
	GetRecMsg(&MsgRecieve,&ReplyFlag);
	if(ReplyFlag==1)	 //如果接收到其他机器发送过来的命令
	{
		ReplyFlag=0;
		//接收返回
		MsgSend.WebID=MsgRecieve.WebID;
		for(i=0;i<9;i++)
			MsgSend.Data[i] =0; //回复的数据初始化
		/*接收命令标识分析*/
			switch(MsgRecieve.CmdNo) //命令标识
			{
				case 0x51: //站点设置小车返回
					SetTaskStop(Task_CallStartNowAGV);
				break;
				case 0x52:  //站点呼叫AGV返回
					SetCallAGVToState2();
				case 0x53:  //站点呼叫AGV返回
					SetCallAGVToState2();
					break;
				case 0x57:  //站点放行AGV返回
					SetTaskStop(Task_GiveWayAGV);
					break;
				case 0x58:  //命令AGV返回原点命令返回
					SetTaskStop(Task_SetBackAGV);
					break;
				case 0x21:  //命令AGV返回原点命令返回
					if(MsgRecieve.Data[0]==CURRENTSTATION)
					{
						MsgSend.CmdNo = 0x22;
						SetAction_CMD(Begin_Charge);
						SetAction_Replay(Begin_Charge);
					}
					break;
				case 0x23:  //命令AGV返回原点命令返回
					if(MsgRecieve.Data[0]==CURRENTSTATION)
					{
						MsgSend.CmdNo = 0x24;
						SetAction_CMD(Stop_Charge);
						SetAction_Replay(Stop_Charge);
					}
					break;
				case 0x40:   
					if(MsgRecieve.Data[0]==CURRENTSTATION)
					{
						SetAction_CMD(BeginZuDang);
						SetAction_Replay(BeginZuDang);
					}
				default:break;   //主控中心停止小车
			}
			if(ReplyFlag==1)
			{
				ReplyFlag=0;
				AddSendMessage(MsgSend);
			}
	}
	if(timeCount>20)
	{
		timeCount=0;
		if(GetMessageNum()>=1)	 //如果本机需要发送命令出去
		{
			PopSendMessage(&MsgSend);
			SendOneMsg    (MsgSend);
		}
	}
}
/*******************************************************************************
 * 名称: InitWirelessMain
 * 功能: 初始化数据交互的处理
 * 形参: 无
 * 返回: 无
 * 说明: 无
 ******************************************************************************/
void InitWirelessMain(u32 baund)
{
	USART2_Init(baund);
}
/*******************************************************************************
 * 名称: WirelessMain
 * 功能: 数据交互的处理
 * 形参: 无
 * 返回: 无
 * 说明: 每隔10ms运行一次
 ******************************************************************************/
void WirelessMain(void)
{
	ZigbeeTranslateMsg();
	AddZigbeeRecTime();
}







