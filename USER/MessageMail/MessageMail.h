#ifndef _MessageMail_H__
#define _MessageMail_H__

#include "wirelessuartdata.h"



unsigned char GetMessageNum(void);  	//��ȡ�ŵ�����Ϣ�ĸ���
void AddSendMessage(MSGSTRUCT MsgSend); //�����Ϣ����������
void PopSendMessage(MSGSTRUCT *MsgPop);  //ȡ����Ϣ����


#endif
