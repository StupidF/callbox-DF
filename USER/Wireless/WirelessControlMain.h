#ifndef _WirelessControlMain_H__
#define _WirelessControlMain_H__

#include "stm32f10x.h"
/*******************************************************************************
 * ����: InitWirelessMain
 * ����: ��ʼ�����ݽ����Ĵ���
 * �β�: ��
 * ����: ��
 * ˵��: ��
 ******************************************************************************/
void InitWirelessMain(u32 baund);
/*******************************************************************************
 * ����: WirelessMain
 * ����: ���ݽ����Ĵ���
 * �β�: ��
 * ����: ��
 * ˵��: ÿ��10ms����һ��
 ******************************************************************************/
void WirelessMain(void);
#endif

