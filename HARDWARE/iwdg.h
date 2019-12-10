#ifndef  _iwdg_h_
#define  _iwdg_h_


#include "stm32f10x_iwdg.h"

void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);

#endif 
