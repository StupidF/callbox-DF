#ifndef _timer_h_
#define _timer_h_


#include "stm32f10x.h"

extern u8 k1;
extern u8 k2;
extern u8 k3;
extern u8 k4;

void TIMER2_Init(void);

/* 自定义宏 */
typedef enum
{
	Tim4_Signal_Clear  = (u8)0x00, 
	Tim4_Signal_Keep   = (u8)0x01,	
	Tim4_Signal_Out    = (u8)0x02
}Tim4_Signal_Typedef;

typedef enum
{
	Tim1_Num_10MS  = (u8)10,
	Tim1_Num_20MS  = (u8)20,
	Tim1_Num_40MS  = (u8)40,
	Tim1_Num_50MS  = (u8)50,
	Tim1_Num_100MS = (u8)100,
	Tim1_Num_200MS = (u8)200,
	Tim1_Num_400MS = (u8)400,
	Tim1_Num_500MS = (u8)500
}Tim1_Num_Typedef;

/*函数声明*/
/*******************************************************************************
 * 名称: Tim4_Init
 * 功能: 定时器4初始化函数
 * 形参: 无
 * 返回: 无
 * 说明: 设定为16分频，向上计数计数周期为1000计数初始值为0
 ******************************************************************************/
void TIMER4_Init(void);

/*******************************************************************************
 * 名称: Tim2_Init
 * 功能: 定时器2初始化函数
 * 形参: 无
 * 返回: 无
 * 说明: 设定为16分频，向下计数计数初值为20000，20ms进入中断
 ******************************************************************************/
void TIMER3_Init(void);
/*******************************************************************************
 * 名称: Tim1_Num_Add
 * 功能: 定时器1中断计数
 * 形参: Tim1_Signal  ==  Tim1_Signal_Clear 将定时器1中断计数清零
 *                Tim1_Signal  ==  Tim1_Signal_Keep定时器1的中断计数将继续增加
 *                Tim1_Signal  ==  Tim1_Signal_Out输出当前的中断计数值
 * 返回: 返回定时器1中断计数的值
 * 说明: 定时器1每1MS进入一次中断
 *                该函数用于计数定时器1进入中断次数
 ******************************************************************************/
u16 Tim4_Num_Add(Tim4_Signal_Typedef Tim4_Signal);

#endif 
