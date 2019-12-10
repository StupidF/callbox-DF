/********************************************************************************
  * 文件名  : button.c 
  * 描述    : Button_Board工程按钮部分库函数
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
*******************************************************************************/

/* 包含自定义头文件 */
#include "key.h"
#include "delay.h"



void DelayTime(u16 DelayTime)
{
	while(DelayTime--);
}

/*函数定义*/
BUTTONSTATE ButtonState[17];//按钮状态
BUTTONSTATE OldButtonState[17];//按钮状态
BUTTONSTATE NewButtonState[17];//按钮状态
/*******************************************************************************
* 名称: Button_Init
* 功能: Button_Board按钮GPIO引脚初始化操作（4位）  拨码开关引脚初始化（4位）
* 形参: 无
* 返回: 无
* 说明: 无 
******************************************************************************/
void Button_Init(void)
{
	/*按钮配置*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE); //使能 PA,PC 端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 			
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; 	//LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure); 					//初始化 GPIOB.5
	
	/*拨码开关引脚的配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; 	//LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure); 					//初始化 GPIOB.5
}
/*******************************************************************************
* 名称: ScandCurrentStation 扫描当前站点
* 功能: 开机扫描当前站点
* 形参: 无
* 返回: 当前站点
* 说明: 无
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
* 名称: KeyScan
* 功能: 按键循环扫描函数
* 形参: 无
* 返回: 无
* 说明: 无
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
* 名称: GetButtonState
* 功能: 获取当前按键信息
* 形参: buttonPos 按键序号
* 返回: 无
* 说明: 无
********************************************************************************/
BUTTONSTATE GetButtonState(u8 buttonPos)
{
	u8 i;
	i=buttonPos;
	return ButtonState[i];
}

