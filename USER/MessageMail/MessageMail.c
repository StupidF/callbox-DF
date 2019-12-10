#include "MessageMail.h"
///通信先进先出信道邮箱管理

MSGSTRUCT MessageBox[10];
unsigned char MessageNum=0;
unsigned char GetMessageNum(void)
{
    return MessageNum;
}
//是否当前信息命令已经在消息邮箱里面了
//返回值 0 没有
//		1 已经收到 
unsigned char IsMessageExist(unsigned char CmdNo)
{
	unsigned char i;
	for(i=0;i<MessageNum;i++)
	{
		if(MessageBox[i].CmdNo==CmdNo)
			return 1;
	}
	return 0;
}

void AddSendMessage(MSGSTRUCT MsgSend)
{
   if(IsMessageExist(MsgSend.CmdNo)==0)
   {
		MessageBox[MessageNum]=MsgSend;
		MessageNum+=1;
		if(MessageNum>=10)
			MessageNum=9;
   }
}

void PopSendMessage(MSGSTRUCT *MsgPop)  //取出信息发送
{
	 unsigned char i;
	*MsgPop=MessageBox[0];
	for(i=0;i<9;i++)
	{
		MessageBox[i]=MessageBox[i+1];
	}
	if(MessageNum>=1)
		MessageNum-=1;
}

