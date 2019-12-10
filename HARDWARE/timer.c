#include "timer.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "stm32f10x.h"
#include "system.h"
 

/**************
ͨ�ö�ʱ��3���ù���
1. ���ö�ʱ������ģʽ
2. ������Ӧ�жϣ�ǰ������Ӧ��
3. �������ʱ��
20ms����һ���ж�
****************/
void TIMER3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //ʹ�ܶ�ʱ��ʱ��    ͨ�ö�ʱ��2���ص�����ʱ����	
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;    //��Ƶϵ��/ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period  = 199;   			//Ԥװ��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;			//����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);     //��������ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;    //��ʱ��3�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ���
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;			//��Ӧ���ȼ���λ
	NVIC_Init(&NVIC_InitStruct);                        //���ò���д����Ӧ�Ĵ���
	
	TIM_Cmd(TIM3,ENABLE);
}

/**************
ͨ�ö�ʱ��2���ù���
1. ���ö�ʱ������ģʽ
2. ������Ӧ�жϣ�ǰ������Ӧ��
3. �������ʱ��
4. 1ms����һ���ж�
****************/
void TIMER4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  			//ʹ�ܶ�ʱ��ʱ��    ͨ�ö�ʱ��2���ص�����ʱ����	
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;    	//��Ƶϵ��/ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;	//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period  = 9;   						//Ԥװ��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;					//����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);     			//��������ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;    		//��ʱ��4�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ���
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;			//��Ӧ���ȼ���λ
	
	NVIC_Init(&NVIC_InitStruct);                        	//���ò���д����Ӧ�Ĵ���
	
	TIM_Cmd(TIM4,ENABLE);
}


/*******************************************************************************
 * ����: Tim4_Num_Add
 * ����: ��ʱ��4�жϼ���
 * �β�: Tim4_Signal  ==  Tim4_Signal_Clear ����ʱ��1�жϼ�������
*        Tim4_Signal  ==  Tim4_Signal_Keep��ʱ��1���жϼ�������������
*        Tim4_Signal  ==  Tim4_Signal_Out�����ǰ���жϼ���ֵ
 * ����: ���ض�ʱ��1�жϼ�����ֵ
 * ˵��: ��ʱ��4ÿ1MS����һ���ж�
 *                �ú������ڼ�����ʱ��1�����жϴ���
 ******************************************************************************/
u16 Tim4_Num_Add(Tim4_Signal_Typedef Tim4_Signal)
{
	static u16 Tim4_Interrupt_Num = 0;
	
	if(Tim4_Signal_Clear == Tim4_Signal) 
		Tim4_Interrupt_Num = 0;
	
	else if(Tim4_Signal_Keep == Tim4_Signal) 
		Tim4_Interrupt_Num ++;
	
	else if(Tim4_Signal_Out  == Tim4_Signal) 
		return Tim4_Interrupt_Num;
	
	return Tim4_Interrupt_Num;
}

/**********************************
��ӦԴ����Ķ�ʱ��2
***********************************/
void TIM3_IRQHandler(void)    
{
	static u8 Tim3_Num = 0;
	Tim3_Num ++;
	/*�����ʱ��2�жϱ�־λ*/
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //��� TIM2 �����жϱ�־
}

/***************************************
��ӦԴ����Ķ�ʱ��1
��ʱ����������תTaskTickMain����
***************************************/
void TIM4_IRQHandler(void)    
{
//	unsigned char i;
	/*�����жϼ���*/
	Tim4_Num_Add(Tim4_Signal_Keep);
	/*����жϱ�־λ*/
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update ); //��� TIM4 �����жϱ�־
	TaskTickMain();
	//LED1=!LED1;
}



