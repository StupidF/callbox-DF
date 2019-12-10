/**
  ******************************************************************************
  * 文件名  : wirelessuartdata.h 
  * 描述    : Button_Board Zigbee串口数据发送函数库头文件
  * 库版本  : V2.1.0
  * 作者    :张锟
  * 修改时间:2013-8-24
  ******************************************************************************
  */

#ifndef __WIRELESSUARTDATA_H
#define __WIRELESSUARTDATA_H

/* 包含系统头文件 */
#include "stm32f10x.h"
#include "CRCDataCale.h"

///AGV小车无线通信模式
#define WIRELESS_ZIGBEE       1   //zigbee无线模块
#define WIRELESS_TRANSPARENT  2   //透明传输模块
#define WIRELESS_PLC_CHARGE   3


//#define WIRELESS_MODE  WIRELESS_ZIGBEE //定义无线通信模块为zigbee模块
#define WIRELESS_MODE WIRELESS_PLC_CHARGE


/* 包含自定义头文件 */

/* 自定义新类型 */
typedef struct _MSGSTRUCT
{
	u8 CmdNo   ; //接收帧命令
	u8 Data[9] ; //接收帧数据
	u16 WebID  ; //接收帧网络
}MSGSTRUCT;

/*******************************************************************************
 * 名称: Uart1_Init
 * 功能: 串口1初始化函数
 * 形参: 无
 * 返回: 无
 * 说明: 波特率:38400 数据位:8 1位停止位无校验位
 ******************************************************************************/
void USART2_Init(u32 baund);

/*******************************************************************************
 * 名称: Uart1_RecOneData
 * 功能: 串口接收到数据的处理
 * 形参: 无
 * 返回: 无
 * 说明: 如果进入了20次该函数ZigbeeUartRecFlag依旧为1则判断其接收
 *                超时这时就放弃该组数据
 ******************************************************************************/
 void Uart2_RecOneData(u8 ZigbeeRecDataTmp);
 
///*******************************************************************************
// * 名称: Uart1_SendOneData
// * 功能: 串口发送一个数据
// * 形参: 无
// * 返回: 无
// * 说明: 如果进入了20次该函数ZigbeeUartRecFlag依旧为1则判断其接收
// *                超时这时就放弃该组数据
// ******************************************************************************/
//void Uart2_SendOneData(void);
 
 /*******************************************************************************
 * 名称: UART1_GetIN_Data
 * 功能: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct
 * 形参: 传入的数组首地址
 * 返回: 无
 * 说明: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct 
 ******************************************************************************/
void RecOneMsg(void);

 /*******************************************************************************
 * 名称: UART1_GetIN_Data
 * 功能: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct
 * 形参: 传入的数组首地址
 * 返回: 无
 * 说明: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct 
 ******************************************************************************/
void SendOneMsg(MSGSTRUCT MsgSend);
 /*******************************************************************************
 * 名称: GetRecMsg
 * 功能: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct
 * 形参: 传入的数组首地址
 * 返回: 无
 * 说明: 将完整接收成功的一帧数据放入结构体指针GetIn_Data_Struct 
 ******************************************************************************/
void GetRecMsg(MSGSTRUCT *MsgRec,unsigned char *RecFlag);
/*******************************************************************************
 * 名称: AddZigbeeRecTime
 * 功能: 接受通讯超时处理函数
 * 形参: 无
 * 返回: 无
 * 说明: 如果进入了20次该函数ZigbeeUartRecFlag依旧为1则判断其接收
 *                超时这时就放弃该组数据
 ******************************************************************************/
void AddZigbeeRecTime(void);

//东风项目阻挡呼叫小车
void SendACall_CAR(void);

#endif


