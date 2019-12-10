#include "led.h"
#include "key.h"

u8 FuctionLEDState[17];   
#define LED_nummax    5
/**********************************************************
* 名称: Button_LED_Init
* 功能: Button_Board按钮指示灯的引脚配置
* 形参: 无
* 返回: 无
* 说明: 配置PA0,PA1,PA4,PA5
********************************************************/
void Button_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 		//使能 Pa 端口时钟
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 	//IO 口速度为 50MHz
	
	GPIO_Init(GPIOA, &GPIO_InitStructure); 					//初始化 GPIO
}


/**********************************
* 名称: SetOnLED,SetOffLED 
* 功能: 改变LED灯状态
* 形参: 无
* 返回: 无
* 说明: 
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
* 名称: HardwareLightONLed
* 功能: Button_Board按钮指示灯的控制
* 形参: 无
* 返回: 无
* 说明: 根据按键返回的信息控制按键对应的指示灯的亮
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
* 名称: HardwareLightONLed
* 功能: Button_Board按钮指示灯的控制
* 形参: 无
* 返回: 无
* 说明: 根据按键返回的信息控制按键对应的指示灯的灭
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
* 名称: Button_Control_LED
* 功能: Button_Board按钮指示灯的控制
* 形参: 无
* 返回: 无
* 说明: 根据按键返回的信息控制按键对应的指示灯的亮灭 
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

