#include "led.h"
#include "key.h"

u8 FuctionLEDState[17];   
#define LED_nummax    5
/**********************************************************
* ����: Button_LED_Init
* ����: Button_Board��ťָʾ�Ƶ���������
* �β�: ��
* ����: ��
* ˵��: ����PA0,PA1,PA4,PA5
********************************************************/
void Button_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		//ʹ�� Pa �˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 	//IO ���ٶ�Ϊ 50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure); 					//��ʼ�� GPIO
}


/**********************************
* ����: SetOnLED,SetOffLED 
* ����: �ı�LED��״̬
* �β�: ��
* ����: ��
* ˵��: 
***********************************/
void SetOnLED(u8 ledPos)
{
	if(ledPos<17)
		FuctionLEDState[ledPos]=1;
}

void SetOffLED(u8 ledPos)
{
	if(ledPos<17)
		FuctionLEDState[ledPos]=0;
}

void SetBiginChargePower(u8 ledPos)
{
	if(ledPos<LED_nummax)
		FuctionLEDState[ledPos]=1;
}

void SetStopChargePower(u8 ledPos)
{
	if(ledPos<LED_nummax)
		FuctionLEDState[ledPos]=1;
}


/**************************************************
* ����: HardwareLightONLed
* ����: Button_Board��ťָʾ�ƵĿ���
* �β�: ��
* ����: ��
* ˵��: ���ݰ������ص���Ϣ���ư�����Ӧ��ָʾ�Ƶ���
***************************************************/
void HardwareLightONLed(u8 pos)
{
	switch(pos)
	{
		case 1:LED1 = 1;break;
		case 2:LED2 = 1;break;
		case 3:LED3 = 1;break;
		case 4:LED4 = 1;break;
		default:break;
	}
}

/**************************************************
* ����: HardwareLightONLed
* ����: Button_Board��ťָʾ�ƵĿ���
* �β�: ��
* ����: ��
* ˵��: ���ݰ������ص���Ϣ���ư�����Ӧ��ָʾ�Ƶ���
***************************************************/
void HardwareLightOFFLed(u8 pos)
{
	switch(pos)
	{
		case 1:LED1 = 0;break;
		case 2:LED2 = 0;break;
		case 3:LED3 = 0;break;
		case 4:LED4 = 0;break;
		default:break;
	}
}

/**************************************************
* ����: Button_Control_LED
* ����: Button_Board��ťָʾ�ƵĿ���
* �β�: ��
* ����: ��
* ˵��: ���ݰ������ص���Ϣ���ư�����Ӧ��ָʾ�Ƶ����� 
**************************************************/
void ButtonLED_Show(void)
{
	u8 i;
	for(i=1;i<=4;i++)
	{
//		if(FuctionLEDState[3] == 0 && GetAction == Begin_Charge)
//		{
//			i=3;
//		}
		if(FuctionLEDState[i]==1)
			HardwareLightONLed(i);
		else
			HardwareLightOFFLed(i);
  }
}
