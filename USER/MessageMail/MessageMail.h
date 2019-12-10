#ifndef _MessageMail_H__
#define _MessageMail_H__

#include "wirelessuartdata.h"



unsigned char GetMessageNum(void);  	//获取信道中消息的个数
void AddSendMessage(MSGSTRUCT MsgSend); //添加信息到发送邮箱
void PopSendMessage(MSGSTRUCT *MsgPop);  //取出信息发送


#endif
