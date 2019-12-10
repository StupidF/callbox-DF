/********************************************************************************
  * �ļ���  : wirelessuartdata.c
  * ����    : Button_Board Zigbee�������ݷ��ͺ�����
  * ��汾  : V2.1.0
  * ����    :���
  * �޸�ʱ��:2013-8-24
*******************************************************************************/

/* �����Զ���ͷ�ļ� */
#include "wirelessuartdata.h"
#include "MessageMail.h"
#include "button_function.h"
#include "delay.h"

unsigned char  ZigbeeUartRec [25];	  //����ͨ�Ž������ݻ���
unsigned char  ZigbeeUartSend[25];	  //����ͨ�ŷ������ݻ���
unsigned char  ZigbeeRecDataTmp=0;
unsigned char  ZigbeeRecDataNum=0;	 //���յ�������
unsigned char  ZigbeeUartRecFlag=0;
unsigned char  ZigbeeUartRecTimeOut=0;
unsigned char  WebSendCount=1;
unsigned char  WebSendPos=0;
MSGSTRUCT      UartRecMsg;    //����ṹ�����
static unsigned char UartRecFlag=0;

/*��������*/
/********USART2_Init
 * ����: ����1��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: ������:�β� ����λ:8 1λֹͣλ��У��λ
 ***************************************************/
void USART2_Init(u32 baund)
{
	GPIO_InitTypeDef   GPIO_InitStruct;     //������������
	USART_InitTypeDef  USART_InitStruct;	//���ڲ�������
	NVIC_InitTypeDef   NVIC_InitStruct;		//�ж�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ�� PC �˿�ʱ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ�ܴ���ʱ��
	


	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0 ; //��ռ���ȼ� 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //�����ȼ� 3
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ��ʹ��
	
	NVIC_Init(&NVIC_InitStruct); //�ж����ȼ���ʼ��
	
	USART_DeInit(USART2); //��λ���� 2

	
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

	
	
	USART_ITConfig(USART2,USART_IT_TC,  DISABLE);	//ʹ�ܷ����ж�
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//ʹ�ܽ����ж�
	
	USART_Cmd(USART2,ENABLE);   //ʹ�ܴ���
	USART_ClearFlag( USART2, USART_FLAG_TXE );
}

/*******************************************************************************
 * ����: Uart2_RecOneData
 * ����: ���ڽ��յ����ݵĴ���
 * �β�: ��
 * ����: ��
 * ˵��: ���������20�θú���ZigbeeUartRecFlag����Ϊ1
		���ж�����ճ�ʱ��ʱ�ͷ�����������
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
 * ����: UART2_GetIN_Data
 * ����: ���������ճɹ���һ֡���ݷ���ṹ��ָ��GetIn_Data_Struct
 * �β�: ����������׵�ַ
 * ����: ��
 * ˵��: ���������ճɹ���һ֡���ݷ���ṹ��ָ��GetIn_Data_Struct 
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


//����С��
void SendACall_CAR(void)
{
	ZigbeeUartSend[0]=0xFD;
	ZigbeeUartSend[1]=0x0A;
	ZigbeeUartSend[2]=0X00;
	ZigbeeUartSend[3]=0; //������ʼͷ
	ZigbeeUartSend[4]=0;
	ZigbeeUartSend[5]=CURRENTSTATION;		//�޸��ұ� Ϊ  ����С��վ���
	ZigbeeUartSend[6]=0x72;
	ZigbeeUartSend[7]=0;
	ZigbeeUartSend[8]=0;
	ZigbeeUartSend[9]=0xff;
	WebSendCount=10; 
	WebSendPos=0;
	USART_ITConfig(USART2,USART_IT_TC, ENABLE);
}
 /*******************************************************************************
 * ����: UART1_GetIN_Data
 * ����: ���������ճɹ���һ֡���ݷ���ṹ��ָ��GetIn_Data_Struct
 * �β�: ����������׵�ַ
 * ����: ��
 * ˵��: ���������ճɹ���һ֡���ݷ���ṹ��ָ��GetIn_Data_Struct 
 ******************************************************************************/
void SendOneMsg(MSGSTRUCT MsgSend)
{
	#if(WIRELESS_MODE==WIRELESS_ZIGBEE)
	unsigned short i,CrcData;
		ZigbeeUartSend[0]=0xFD;
		ZigbeeUartSend[1]=0x0D;
		ZigbeeUartSend[2]=(MsgSend.WebID>>8)&0XFF;
		ZigbeeUartSend[3]=MsgSend.WebID&0XFF;
		ZigbeeUartSend[4]=0x0F; //������ʼͷ
		ZigbeeUartSend[5]=MsgSend.CmdNo;
		for(i=0;i<9;i++)
			ZigbeeUartSend[i+6]=MsgSend.Data[i];
//		CrcData=crc16(&ZigbeeUartSend[4],11);
		ZigbeeUartSend[15]=(CrcData>>8)&0XFF;
		ZigbeeUartSend[16]=(CrcData)&0XFF;
		WebSendCount=17; 
		WebSendPos=0;
		USART_ITConfig(USART2,USART_IT_TC,  DISABLE);	//ʹ�ܷ����ж�

	#elif(WIRELESS_MODE==WIRELESS_TRANSPARENT)
		unsigned short i;
			//,CrcData;
		ZigbeeUartSend[0]=0xFD;
		ZigbeeUartSend[1]=0x10;
		ZigbeeUartSend[2]= 0x20;
		ZigbeeUartSend[3]=0x0F; //������ʼͷ
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
		ZigbeeUartSend[3]=0; //������ʼͷ
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
		ZigbeeUartSend[4]=0x0F; //������ʼͷ
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
 * ����: GetRecMsg
 * ����: ���������ճɹ���һ֡���ݷ���ṹ��ָ��GetIn_Data_Struct
 * �β�: ����������׵�ַ
 * ����: ��
 * ˵��: ���������ճɹ���һ֡���ݷ���ṹ��ָ��GetIn_Data_Struct 
 ******************************************************************************/
void GetRecMsg(MSGSTRUCT *MsgRec,unsigned char *RecFlag)
{
//��ȡ���յ�������֡�����*RecFlag Ϊ1�������ɹ���ȡ��һ֡����
	if(UartRecFlag==1)
	{
		*MsgRec=UartRecMsg;
		*RecFlag=1;
		UartRecFlag=0;
	}
}

/*******************************************************************************
 * ����: AddZigbeeRecTime
 * ����: ����ͨѶ��ʱ������
 * �β�: ��
 * ����: ��
 * ˵��: ���������20�θú���ZigbeeUartRecFlag����Ϊ1���ж������
 *                ��ʱ��ʱ�ͷ�����������
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
	/*ȡ�����յ�������*/
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
//                ComSendLength = 0;   //����յ�ַƫ�ƼĴ���
//                sendPosi = 0;
//                USART_ITConfig( UART4, USART_IT_TC, DISABLE );
//            }

//            USART_ClearITPendingBit( UART4, USART_IT_TC );
//        }
//    }

//    UART4->DR;
//    UART4->SR;


