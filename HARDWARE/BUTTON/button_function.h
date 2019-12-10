/**
  ******************************************************************************
  * �ļ���  : button_function.h
  * ����    : Button_Board���̰�ťָʾ�ƿ⺯��ͷ�ļ�
  * ��汾  : V2.1.0
  * ����    :���
  * �޸�ʱ��:2013-8-24
  ******************************************************************************
  */

#ifndef __BUTTON_FUNCTION_H
#define __BUTTON_FUNCTION_H

/* ����ϵͳͷ�ļ� */
#include "stm32f10x.h"

/* �����Զ���ͷ�ļ� */
#include "key.h"
extern unsigned char CURRENTSTATION;

/*******************************************************************************
* ����: InitButtonFunction
* ����: InitButtonFunction��ʼ������
* �β�: ��
* ����: ��
* ˵��: ǿ�����������͵�ƽ 
******************************************************************************/
void InitButtonFunction(void);
/*******************************************************************************
* ����: SetTaskRun,SetTaskStop
* ����: SetTaskRun,SetTaskStop ��ʼ ֹͣ����
* �β�: ��
* ����: ��
* ˵��: ǿ�����������͵�ƽ 
******************************************************************************/
void SetTaskRun(TASKNO taskNo,BUTTONPOS pos); //����ִ������
void SetTaskStop(TASKNO taskNo);//����ֹͣ����

void SetCallAGVToState2(void); //����վ�����AGV�Ľ׶�Ϊ2�׶�
/*******************************************************************************
* ����: TaskCallAGV
* ����: TaskCallAGV ������ϸ
* �β�: ��
* ����: ��
* ˵��: ǿ�����������͵�ƽ 
******************************************************************************/
void TaskCallAGV(void); //����AGV
void TaskGiveWayAGV(void);//����AGV
void TaskSetBackAGV(void); //����AGV����ԭ��
/*******************************************************************************
* ����: TaskRun
* ����: TaskCallAGV ������ϸ
* �β�: ��
* ����: ��
* ˵��: ǿ�����������͵�ƽ 
******************************************************************************/
void TaskRun(TASKNO taskNo);
/*******************************************************************************
* ����: ButtonFunctionMain
* ����: ButtonFunctionMain��ť����������
* �β�: ��
* ����: ��
* ˵��: 20msִ��һ��
******************************************************************************/
void ButtonFunctionMain(void);

//���ָ��
void SetAction_CMD(Action act_cmd);
//���ָ��
void ClearAction_CMD(void );
//��綯��ִ��
void Action_CMD(void);
Action GetAction(void);
void SetAction_Replay(Action act_cmd);
void ClearAction_Replay(Action act_cmd);
#endif
