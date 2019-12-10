/********************************************************************************
  * 文件名  : wirelessuartdata.c
  * 描述    : Button_Board Zigbee串口数据发送函数库
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
*******************************************************************************/

/* 包含自定义头文件 */
#include "wirelessuartdata.h"
#include "MessageMail.h"
#include "button_function.h"
#include "delay.h"

unsigned char  ZigbeeUartRec [25];	  //无线通信接收数据缓存
unsigned char  ZigbeeUartSend[25];	  //无线通信发送数据缓存
unsigned char  ZigbeeRecDataTmp=0;
unsigned char  ZigbeeRecDataNum=0;	 //接收到数据数
unsigned char  ZigbeeUartRecFlag=0;
unsigned char  ZigbeeUartRecTimeOut=0;
unsigned char  WebSendCount=1;
unsigned char  WebSendPos=0;
MSGSTRUCT      UartRecMsg;    //定义结构体变量
static unsigned char UartRecFlag=0;

/*函数定义*/
/********USART2_Init
 * 功能: 串口1初始化函数
 * 形参: 无
 * 返回: 无
 * 说明: 波特率:形参 数据位:8 1位停止位无校验位
 ***************************************************/
void USART2_Init(u32 baund)
{
	GPIO_InitTypeDef   GPIO_InitStruct;     //串口引脚配置
	USART_InitTypeDef  USART_InitStruct;	//串口参数配置
	NVIC_InitTypeDef   NVIC_InitStruct;		//中断配置

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能 PC 端口时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能串口时钟
	


	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //子优先级 3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	
	NVIC_Init(&NVIC_InitStruct); //中断优先级初始化
	
	USART_DeInit(USART2); //复位串口 2

	
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitStruct.USART_BaudRate= baund;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits= USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);

	
	
	USART_ITConfig(USART2,USART_IT_TC,  DISABLE);	//使能发送中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//使能接收中断
	
	USART_Cmd(USART2,ENABLE);   //使能串口
	USART_ClearFlag( USART2, USART_FLAG_TXE );
}

/*******************************************************************************
 * 名称: Uart2_RecOneData
 * 功能: 串口接收到数据的处理
 * 形参: 无
 * 返回: 无
 * 说明: 如果进入了20次该函数ZigbeeUartRecFlag依旧为1
		则判断其接收超时这时就放弃该组数据
 ******************************************************************************/
void USART2_RecOneData(u8 ZigbeeRecDataTmp)
{
	#if(WIRELESS_MODE==WIRELESS_ZIGBEE)
		if(ZigbeeUartRecFlag==0)
			{
				ZigbeeUartRecTimeOut=0; 
				ZigbeeRecDataNum=0;
				if(ZigbeeRecDataTmp==0xFD)
				{
					ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
					ZigbeeUartRecFlag=1;
				}
			}
			else 
			{
				if(ZigbeeRecDataNum==1)
				{
					if(ZigbeeRecDataTmp!=0x0D)
					
					{
						ZigbeeUartRecFlag=0;
					}
				}
				ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
				if(ZigbeeRecDataNum==0x13)
				{	
					RecOneMsg();
					ZigbeeRecDataNum=0;
					ZigbeeUartRecFlag=0;
				}
			}
	#elif(WIRELESS_MODE==WIRELESS_TRANSPARENT)
		unsigned short recWebID=0;
		if(ZigbeeUartRecFlag==0)
		{
			ZigbeeUartRecTimeOut=0; 
			ZigbeeRecDataNum=0;
			if(ZigbeeRecDataTmp==0xFD)
			{
			 	ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
				ZigbeeUartRecFlag=1;
			}
		}
		else 
		{ 
			if(ZigbeeRecDataNum==1)
			{
				if(ZigbeeRecDataTmp!=0x10)
				{
					ZigbeeUartRecFlag=0;
					return;
				}
			}
/*			if(ZigbeeRecDataNum==3)
			{
				recWebID=ZigbeeUartRec[2];
				recWebID=recWebID<<8;
				recWebID+=ZigbeeRecDataTmp;
				if(recWebID!=CURRENTSTATION&&recWebID!=0xFFFF)
				{
					ZigbeeUartRecFlag=0;
					return;
				}
			}*/
			ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
			if(ZigbeeRecDataNum==0x10)
			{	
				if(ZigbeeRecDataTmp == 0xff)
					RecOneMsg();
				ZigbeeRecDataNum=0;
				ZigbeeUartRecFlag=0;
			}
		} 
	#elif(WIRELESS_MODE==WIRELESS_PLC_CHARGE)
	unsigned short recWebID=0;
		if(ZigbeeUartRecFlag==0)
		{
			ZigbeeUartRecTimeOut=0; 
			ZigbeeRecDataNum=0;
			if(ZigbeeRecDataTmp==0xFD)
			{
			 	ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
				ZigbeeUartRecFlag=1;
			}
		}
		else 
		{ 
			if(ZigbeeRecDataNum==1)
			{
				if(ZigbeeRecDataTmp!=0x0a)
				{
					ZigbeeUartRecFlag=0;
					return;
				}
			}
			ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
			if(ZigbeeRecDataNum==0x0a)
			{	
				if(ZigbeeRecDataTmp == 0xff)
					RecOneMsg();
				ZigbeeRecDataNum=0;
				ZigbeeUartRecFlag=0;
			}
		} 
	#else
	if(ZigbeeUartRecFlag==0)
	{ 
		ZigbeeUartRecTimeOut=0; 
		ZigbeeRecDataNum=0;
		if(ZigbeeRecDataTmp==0xFD)
		{
			ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
			ZigbeeUartRecFlag=1;
		}
	} 
	else
	{ 
		if(ZigbeeRecDataNum==1)
		{
			if(ZigbeeRecDataTmp!=0x0D)
			{
				ZigbeeUartRecFlag=0;
			}
		}
		ZigbeeUartRec[ZigbeeRecDataNum++]=ZigbeeRecDataTmp;
		if(ZigbeeRecDataNum==0x13)
		{
			RecOneMsg();
			ZigbeeRecDataNum=0;
			ZigbeeUartRecFlag=0;
		}
	}
	#endif
}
 
 /*******************************************************************************
 * 名称: UART2_GetIN_Data
 * 功能: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct
 * 形参: 传入的数组首地址
 * 返回: 无
 * 说明: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct 
 ******************************************************************************/
void RecOneMsg(void)
{
	unsigned short CrcData=0,CrcCaleData=0,tmp;
	unsigned char i;
//   	CrcData=ZigbeeUartRec[15];
//	CrcData<<=8;
//	CrcData+=ZigbeeUartRec[16];
//	CrcCaleData=crc16(&ZigbeeUartRec[4],11);
//	if(CrcCaleData==CrcData)
//	{
		UartRecMsg.CmdNo=ZigbeeUartRec[6];
		for(i=0;i<3;i++)
			UartRecMsg.Data[i]=ZigbeeUartRec[i+5];
		tmp=ZigbeeUartRec[17];
		tmp<<=8;
		tmp=tmp+ZigbeeUartRec[18];
		UartRecMsg.WebID=tmp;
		UartRecFlag=1;
//	}
}


//呼叫小车
void SendACall_CAR(void)
{
	ZigbeeUartSend[0]=0xFD;
	ZigbeeUartSend[1]=0x0A;
	ZigbeeUartSend[2]=0X00;
	ZigbeeUartSend[3]=0; //数据起始头
	ZigbeeUartSend[4]=0;
	ZigbeeUartSend[5]=CURRENTSTATION;		//修改右边 为  呼叫小车站点号
	ZigbeeUartSend[6]=0x72;
	ZigbeeUartSend[7]=0;
	ZigbeeUartSend[8]=0;
	ZigbeeUartSend[9]=0xff;
	WebSendCount=10; 
	WebSendPos=0;
	USART_ITConfig(USART2,USART_IT_TC, ENABLE);
}
 /*******************************************************************************
 * 名称: UART1_GetIN_Data
 * 功能: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct
 * 形参: 传入的数组首地址
 * 返回: 无
 * 说明: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct 
 ******************************************************************************/
void SendOneMsg(MSGSTRUCT MsgSend)
{
	#if(WIRELESS_MODE==WIRELESS_ZIGBEE)
	unsigned short i,CrcData;
		ZigbeeUartSend[0]=0xFD;
		ZigbeeUartSend[1]=0x0D;
		ZigbeeUartSend[2]=(MsgSend.WebID>>8)&0XFF;
		ZigbeeUartSend[3]=MsgSend.WebID&0XFF;
		ZigbeeUartSend[4]=0x0F; //数据起始头
		ZigbeeUartSend[5]=MsgSend.CmdNo;
		for(i=0;i<9;i++)
			ZigbeeUartSend[i+6]=MsgSend.Data[i];
//		CrcData=crc16(&ZigbeeUartSend[4],11);
		ZigbeeUartSend[15]=(CrcData>>8)&0XFF;
		ZigbeeUartSend[16]=(CrcData)&0XFF;
		WebSendCount=17; 
		WebSendPos=0;
		USART_ITConfig(USART2,USART_IT_TC,  DISABLE);	//使能发送中断

	#elif(WIRELESS_MODE==WIRELESS_TRANSPARENT)
		unsigned short i;
			//,CrcData;
		ZigbeeUartSend[0]=0xFD;
		ZigbeeUartSend[1]=0x10;
		ZigbeeUartSend[2]= 0x20;
		ZigbeeUartSend[3]=0x0F; //数据起始头
		ZigbeeUartSend[4]=MsgSend.CmdNo;
		for(i=0;i<9;i++)
			ZigbeeUartSend[i+5]=MsgSend.Data[i];
//		CrcData=crc16(&ZigbeeUartSend[4],11);
		ZigbeeUartSend[14]= CURRENTSTATION;
		ZigbeeUartSend[15]= 0xFF;
		WebSendCount=16; 
		WebSendPos=0;
		USART_ITConfig(USART2,USART_IT_TC, ENABLE);
	#elif(WIRELESS_MODE==WIRELESS_PLC_CHARGE)
		ZigbeeUartSend[0]=0xFD;
		ZigbeeUartSend[1]=0x0A;
		ZigbeeUartSend[2]=0X00;
		ZigbeeUartSend[3]=0; //数据起始头
		ZigbeeUartSend[4]=0;
		ZigbeeUartSend[5]=CURRENTSTATION;
		ZigbeeUartSend[6]=MsgSend.CmdNo;
		ZigbeeUartSend[7]=0;
		ZigbeeUartSend[8]=0;
		ZigbeeUartSend[9]=0xff;
		WebSendCount=10; 
		WebSendPos=0;
		USART_ITConfig(USART2,USART_IT_TC, ENABLE);
	#else
	unsigned short i,CrcData;
		ZigbeeUartSend[0]=0xFD;
		ZigbeeUartSend[1]=0x0D;
		ZigbeeUartSend[2]=(MsgSend.WebID>>8)&0XFF;
		ZigbeeUartSend[3]=MsgSend.WebID&0XFF;
		ZigbeeUartSend[4]=0x0F; //数据起始头
		ZigbeeUartSend[5]=MsgSend.CmdNo;
		for(i=0;i<9;i++)
			ZigbeeUartSend[i+6]=MsgSend.Data[i];
//		CrcData=crc16(&ZigbeeUartSend[4],11);
		ZigbeeUartSend[15]=(CrcData>>8)&0XFF;
		ZigbeeUartSend[16]=(CrcData)&0XFF;
		WebSendCount=17;
		WebSendPos=0;
		USART_ITConfig(USART2,USART_IT_TC, ENABLE);

	#endif
}

 /*******************************************************************************
 * 名称: GetRecMsg
 * 功能: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct
 * 形参: 传入的数组首地址
 * 返回: 无
 * 说明: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct 
 ******************************************************************************/
void GetRecMsg(MSGSTRUCT *MsgRec,unsigned char *RecFlag)
{
//获取接收到的数据帧，如果*RecFlag 为1，则代表成功收取到一帧数据
	if(UartRecFlag==1)
	{
		*MsgRec=UartRecMsg;
		*RecFlag=1;
		UartRecFlag=0;
	}
}

/*******************************************************************************
 * 名称: AddZigbeeRecTime
 * 功能: 接受通讯超时处理函数
 * 形参: 无
 * 返回: 无
 * 说明: 如果进入了20次该函数ZigbeeUartRecFlag依旧为1则判断其接收
 *                超时这时就放弃该组数据
 ******************************************************************************/
void AddZigbeeRecTime(void)	
{
	if(ZigbeeUartRecFlag!=0)
	{
		ZigbeeUartRecTimeOut++;
		if(ZigbeeUartRecTimeOut>20)
			ZigbeeUartRecFlag=0;
	}
	else
		ZigbeeUartRecTimeOut=0;
}

void USART2_IRQHandler(void)
{
	/*取出接收到的数据*/
	if(USART_GetFlagStatus(USART2, USART_IT_RXNE) != RESET)  
	{
		USART2_RecOneData(USART_ReceiveData(USART2));
		USART_ClearFlag(USART2,USART_IT_RXNE);
	}
//	ClrBit(UART1->SR,6);
	//else if()
	if ( USART_GetITStatus( USART2, USART_IT_TC ) != RESET )
	{
		if(WebSendPos<WebSendCount)
		{	
			USART_SendData(USART2,ZigbeeUartSend[WebSendPos]);
			WebSendPos++;	
		}
		else
		{
			WebSendPos=0;
			WebSendCount=0;
			USART_ITConfig(USART2,USART_IT_TC, DISABLE);
		}	
	}	
}

//                WebSendPos += 1;
//                USART_SendData( UART4, ZigbeeUartSend[WebSendPos] );
//            }
//            else
//            {
//                USART_ITConfig( UART4, USART_IT_TC, DISABLE );
//            }

//            USART_ClearITPendingBit( UART4, USART_IT_TC );
//        }
//    }
//    else
//    {
//        if ( USART_GetITStatus( UART4, USART_IT_RXNE ) != RESET )
//        {
//            ComReachTime = 0;
//            ZigbeeRecDataTmp = USART_ReceiveData( UART4 );

//            /* Read one byte from the receive data register */
//            ///////////////////////////////////////////////////////////////////////////
//            if ( ComReachLength < MAXCOMDATA )
//            {
//                ComReachBuff[ComReachLength] = ZigbeeRecDataTmp;
//            }

//            ComReachLength += 1;
//            ////////////////////////////////////////////////////////////////////////////
//            USART_ClearITPendingBit( UART4, USART_IT_RXNE );
//        }

//        if ( USART_GetITStatus( UART4, USART_IT_TC ) != RESET )
//        {
//            /* Read one byte from the receive data register */
//            if ( sendPosi < ComSendLength )
//            {
//                sendPosi++;
//                USART_SendData( UART4, ComSendBuff[sendPosi] );
//            }
//            else
//            {
//                ComSendLength = 0;   //清接收地址偏移寄存器
//                sendPosi = 0;
//                USART_ITConfig( UART4, USART_IT_TC, DISABLE );
//            }

//            USART_ClearITPendingBit( UART4, USART_IT_TC );
//        }
//    }

//    UART4->DR;
//    UART4->SR;


