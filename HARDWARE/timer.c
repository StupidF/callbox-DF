#include "timer.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "stm32f10x.h"
#include "system.h"
 

/**************
通用定时器3配置过程
1. 配置定时器工作模式
2. 配置相应中断（前后必须对应）
3. 最后开启定时器
20ms出发一次中断
****************/
void TIMER3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  //使能定时器时钟    通用定时器2挂载到低速时钟线	
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;    //分频系数/时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;	//向上计数
	TIM_TimeBaseInitStruct.TIM_Period  = 199;   			//预装数
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;			//设置时钟频率除数的预分频值
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);     //允许跟新中断
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;    //定时器3中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;	//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级无
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;			//响应优先级三位
	NVIC_Init(&NVIC_InitStruct);                        //配置参数写入相应寄存器
	
	TIM_Cmd(TIM3,ENABLE);
}

/**************
通用定时器2配置过程
1. 配置定时器工作模式
2. 配置相应中断（前后必须对应）
3. 最后开启定时器
4. 1ms进入一次中断
****************/
void TIMER4_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  			//使能定时器时钟    通用定时器2挂载到低速时钟线	
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision =  TIM_CKD_DIV1;    	//分频系数/时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode  = TIM_CounterMode_Up;	//向上计数
	TIM_TimeBaseInitStruct.TIM_Period  = 9;   						//预装数
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;					//设置时钟频率除数的预分频值
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);     			//允许跟新中断
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;    		//定时器4中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级无
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;			//响应优先级三位
	
	NVIC_Init(&NVIC_InitStruct);                        	//配置参数写入相应寄存器
	
	TIM_Cmd(TIM4,ENABLE);
}


/*******************************************************************************
 * 名称: Tim4_Num_Add
 * 功能: 定时器4中断计数
 * 形参: Tim4_Signal  ==  Tim4_Signal_Clear 将定时器1中断计数清零
*        Tim4_Signal  ==  Tim4_Signal_Keep定时器1的中断计数将继续增加
*        Tim4_Signal  ==  Tim4_Signal_Out输出当前的中断计数值
 * 返回: 返回定时器1中断计数的值
 * 说明: 定时器4每1MS进入一次中断
 *                该函数用于计数定时器1进入中断次数
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
对应源程序的定时器2
***********************************/
void TIM3_IRQHandler(void)    
{
	static u8 Tim3_Num = 0;
	Tim3_Num ++;
	/*清除定时器2中断标志位*/
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM2 更新中断标志
}

/***************************************
对应源程序的定时器1
定时进行任务跳转TaskTickMain（）
***************************************/
void TIM4_IRQHandler(void)    
{
//	unsigned char i;
	/*增加中断计数*/
	Tim4_Num_Add(Tim4_Signal_Keep);
	/*清空中断标志位*/
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update ); //清除 TIM4 更新中断标志
	TaskTickMain();
	//LED1=!LED1;
}



