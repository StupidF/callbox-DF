#include "MessageMail.h"
///ͨ���Ƚ��ȳ��ŵ��������

MSGSTRUCT MessageBox[10];
unsigned char MessageNum=0;
unsigned char GetMessageNum(void)
{
    return MessageNum;
}
//�Ƿ�ǰ��Ϣ�����Ѿ�����Ϣ����������
//����ֵ 0 û��
//		1 �Ѿ��յ� 
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

void PopSendMessage(MSGSTRUCT *MsgPop)  //ȡ����Ϣ����
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

