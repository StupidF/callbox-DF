#ifndef _led_h_
#define _led_h_

#include "stm32f10x.h"
#include "sys.h"

#define LED1   PAout(5)
#define LED2   PAout(4)
#define LED3   PAout(1)
#define LED4   PAout(0)

void Button_LED_Init(void);
extern void SetOnLED(u8 ledPos);
extern void SetOffLED(u8 ledPos);
void ButtonLED_Show(void);


void SetStopChargePower(u8  ledPos);	 //停止充电
void SetBiginChargePower(u8 ledPos); 	//开始充电


void HardwareLightONLed(u8 pos);
void HardwareLightOFFLed(u8 pos);


#endif
