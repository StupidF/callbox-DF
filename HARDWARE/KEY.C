/********************************************************************************
  * �ļ���  : button.c 
  * ����    : Button_Board���̰�ť���ֿ⺯��
  * ��汾  : V2.1.0
  * ����    :���
  * �޸�ʱ��:2013-8-24
*******************************************************************************/

/* �����Զ���ͷ�ļ� */
#include "key.h"
#include "delay.h"



void DelayTime(u16 DelayTime)
{
	while(DelayTime--);
}

/*��������*/
BUTTONSTATE ButtonState[17];//��ť״̬
BUTTONSTATE OldButtonState[17];//��ť״̬
BUTTONSTATE NewButtonState[17];//��ť״̬
/*******************************************************************************
* ����: Button_Init
* ����: Button_Board��ťGPIO���ų�ʼ��������4λ��  ���뿪�����ų�ʼ����4λ��
* �β�: ��
* ����: ��
* ˵��: �� 
******************************************************************************/
void Button_Init(void)
{
	/*��ť����*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE); //ʹ�� PA,PC �˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 			
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 	//LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure); 					//��ʼ�� GPIOB.5
	
	/*���뿪�����ŵ�����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 	//LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure); 					//��ʼ�� GPIOB.5
}
/*******************************************************************************
* ����: ScandCurrentStation ɨ�赱ǰվ��
* ����: ����ɨ�赱ǰվ��
* �β�: ��
* ����: ��ǰվ��
* ˵��: ��
******************************************************************************/
unsigned char ScandCurrentStation(void)
{
	u8 tmp=0,tmp1=0,station=0;
	while(station==0)
	{
		tmp=~(GPIO_ReadInputData(GPIOC))&0x0f;
		delay_ms(10);
		tmp1=~(GPIO_ReadInputData(GPIOC))&0x0f;
		station=0;
		if(tmp==tmp1)
		{
//			if((tmp&0x10)!=0)
//				station|=0x10;
			if((tmp&0x08)!=0)
				station|=0x08;
			if((tmp&0x04)!=0)
				station|=0x04;
			if((tmp&0x02)!=0)
				station|=0x02;
			if((tmp&0x01)!=0)
				station|=0x01;
			return station;
		}
	}
	return 0;
}
/*******************************************************************************
* ����: KeyScan
* ����: ����ѭ��ɨ�躯��
* �β�: ��
* ����: ��
* ˵��: ��
********************************************************************************/
void ButtonScan(void)
{
	u8 i,m;
	u8 KeyState1,KeyState2;
	KeyState1=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
		KeyState1|=0x01<<0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
		KeyState1|=0x01<<1;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==0)
		KeyState1|=0x01<<2;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0)
		KeyState1|=0x01<<3;
//	if(GPIO_ReadInputDataBit(Button_PIN_5)==0)
//		KeyState1|=0x01<<4;
//	if(GPIO_ReadInputDataBit(Button_PIN_6)==0)
//		KeyState1|=0x01<<5;
//	if(GPIO_ReadInputDataBit(Button_PIN_7)==0)
//		KeyState1|=0x01<<6;
	delay_ms(10);
	KeyState2=0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
		KeyState2|=0x01<<0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
		KeyState2|=0x01<<1;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==0)
		KeyState2|=0x01<<2;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==0)
		KeyState2|=0x01<<3;
//	if(GPIO_ReadInputDataBit(Button_PIN_5)==0)
//		KeyState2|=0x01<<4;
//	if(GPIO_ReadInputDataBit(Button_PIN_6)==0)
//		KeyState2|=0x01<<5;
//	if(GPIO_ReadInputDataBit(Button_PIN_7)==0)
//		KeyState2|=0x01<<6;
	if(KeyState1==KeyState2)
	{
		for(i=0;i<16;i++)
		{
			m=1<<i;
			if((KeyState1&m)!=0)
				NewButtonState[i+1]=BUTTON_ON;
			else
				NewButtonState[i+1]=BUTTON_OFF;
		}
		
		for(i=0;i<17;i++)
		{
			if(OldButtonState[i]==BUTTON_ON&&NewButtonState[i]==BUTTON_OFF)
			{
				ButtonState[i]=BUTTON_ON_TO_OFF;
			}
			
			else if(OldButtonState[i]==BUTTON_OFF&&NewButtonState[i]==BUTTON_ON)
			{
				ButtonState[i]=BUTTON_OFF_TO_ON;
			}
			else
			{
				ButtonState[i]=NewButtonState[i];
			}
			OldButtonState[i]=NewButtonState[i];
		}
	}
}

/*******************************************************************************
* ����: GetButtonState
* ����: ��ȡ��ǰ������Ϣ
* �β�: buttonPos �������
* ����: ��
* ˵��: ��
********************************************************************************/
BUTTONSTATE GetButtonState(u8 buttonPos)
{
	u8 i;
	i=buttonPos;
	return ButtonState[i];
}
