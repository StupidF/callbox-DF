#include "WirelessControlMain.h"
#include "WirelessUartData.h"
#include "MessageMail.h"
#include "key.h"
#include "button_function.h"
#include "led.h"

MSGSTRUCT   MsgRecieve;
MSGSTRUCT   MsgSend;

void ZigbeeTranslateMsg(void)		//������յ������ݴ�
{	
	static unsigned short timeCount=0;
 	static unsigned char ReplyFlag=0,i;
	ReplyFlag=0;
	timeCount++;
	GetRecMsg(&MsgRecieve,&ReplyFlag);
	if(ReplyFlag==1)	 //������յ������������͹���������
	{
		ReplyFlag=0;
		//���շ���
		MsgSend.WebID=MsgRecieve.WebID;
		for(i=0;i<9;i++)
			MsgSend.Data[i] =0; //�ظ������ݳ�ʼ��
		/*���������ʶ����*/
			switch(MsgRecieve.CmdNo) //�����ʶ
			{
				case 0x51: //վ������С������
					SetTaskStop(Task_CallStartNowAGV);
				break;
				case 0x52:  //վ�����AGV����
					SetCallAGVToState2();
				case 0x53:  //վ�����AGV����
					SetCallAGVToState2();
					break;
				case 0x57:  //վ�����AGV����
					SetTaskStop(Task_GiveWayAGV);
					break;
				case 0x58:  //����AGV����ԭ�������
					SetTaskStop(Task_SetBackAGV);
					break;
				case 0x21:  //����AGV����ԭ�������
					if(MsgRecieve.Data[0]==CURRENTSTATION)
					{
						MsgSend.CmdNo = 0x22;
						SetAction_CMD(Begin_Charge);
						SetAction_Replay(Begin_Charge);
					}
					break;
				case 0x23:  //����AGV����ԭ�������
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
				default:break;   //��������ֹͣС��
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
		if(GetMessageNum()>=1)	 //���������Ҫ���������ȥ
		{
			PopSendMessage(&MsgSend);
			SendOneMsg    (MsgSend);
		}
	}
}
/*******************************************************************************
 * ����: InitWirelessMain
 * ����: ��ʼ�����ݽ����Ĵ���
 * �β�: ��
 * ����: ��
 * ˵��: ��
 ******************************************************************************/
void InitWirelessMain(u32 baund)
{
	USART2_Init(baund);
}
/*******************************************************************************
 * ����: WirelessMain
 * ����: ���ݽ����Ĵ���
 * �β�: ��
 * ����: ��
 * ˵��: ÿ��10ms����һ��
 ******************************************************************************/
void WirelessMain(void)
{
	ZigbeeTranslateMsg();
	AddZigbeeRecTime();
}







