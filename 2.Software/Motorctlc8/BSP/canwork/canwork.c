#include "canwork.h" //库文件声明
#include "canworkconf.h"
#if (CANWORK_ENABLE)
#include "main.h"
#include <string.h>//用于字符串处理的库
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>



CAN_TxHeaderTypeDef     TxMeg;//CAN发送设置相关结构体
CAN_RxHeaderTypeDef     RxMeg;//CAN接收设置相关结构体

uint8_t CAN_RX_BUF[CAN_REC_LEN];//接收缓冲,最大CAN1_REC_LEN个字节.末字节为换行符
uint16_t CAN_RX_STA;//接收状态标记

void  HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)  //接收回调函数
{
    uint8_t  Data[8],i;//接收缓存数组
    HAL_StatusTypeDef ret;//判断状态的枚举
	ret=HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO1,&RxMeg,Data);//接收邮箱中的数据
	if (HAL_OK==ret){//判断接收是否成功
		//接收成功后的数据处理程序，写在此处。（数据在Data数组中）
		//以下2行是采用简单的寄存器查寻方式处理接收数据，每次只接收1位。在实际项目中的复杂接收程序可自行编写。
		CAN_RX_BUF[0]=Data[0];//将接收到的数据放入缓存数组（因只用到1个数据，所以只存放在数据[0]位置）
		CAN_RX_STA++;//数据接收标志位加1
		printf("\nGet Rx Message Success!!\r\nData:");
		for(i=0; i<8; i++)
			   printf("%d", Data[i]);
	}
}
//CAN发送数据函数（参数：总线名，ID，数据数组，数量。返回值：0成功HAL_OK，1参数错误HAL_ERROR，2发送失败HAL_BUSY）
//示例：CAN1_SendNormalData(&hcan1,0,CAN_buffer,8);//CAN发送数据函数
uint8_t  CAN_SendNormalData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t  Len)
{
    HAL_StatusTypeDef HAL_RetVal;//判断状态的枚举
    uint16_t SendTimes,SendCNT=0;
    uint8_t  FreeTxNum=0;
    uint8_t  FreeLevelCount=0;
    uint32_t CAN_TX_BOX0;
    TxMeg.StdId=ID;
    TxMeg.IDE = CAN_ID_STD;//扩展帧标识（STD标准帧/EXT扩展帧）
    TxMeg.RTR = CAN_RTR_DATA;//远程帧标识（DATA数据帧/REMOTE远程帧）
    if(!hcan||!pData||!Len){
    	printf("\n\rCAN send fail！\n\r"); //串口发送
    	return  HAL_ERROR;//如果总线名、数据、数量任何一个为0则返回值为1
    }
    SendTimes=Len/8+(Len%8?1:0);
    FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(hcan);//得出空闲邮箱的数量
    TxMeg.DLC=8;
    while(SendTimes--){//循环判断分批发送是否结束
       if(0==SendTimes){//如果分批发送结束
           if(Len%8)TxMeg.DLC=Len%8;//则加入最后不足8个的数据内容
       }
       FreeLevelCount = 0;//防止死循环
       while(0 == FreeTxNum&&FreeLevelCount<10){
    	   	FreeLevelCount++;
    	   	HAL_Delay(1);
            FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
        }
       HAL_Delay(1);//延时防止速度过快导致的发送失败
       //开始发送数据（参数：总线名，设置参数，数据，邮箱号）
       HAL_RetVal=HAL_CAN_AddTxMessage(hcan,&TxMeg,pData+SendCNT,&CAN_TX_BOX0);
       if(HAL_RetVal!=HAL_OK){
    		   printf("\n\rCAN bus busy！\n\r"); //串口发送
    		   return  HAL_BUSY;//如果发送失败，则返回值为2
       }
       SendCNT+=8;
    }
    return HAL_OK;//如果发送成功结束，返回值为0
}
//CAN总线通信，使用CAN1，这是CAN专用的printf函数
//调用方法：CAN1_printf("123"); //向UART8发送字符123
void CAN_printf (char *fmt, ...)
{
    char buff[CAN_REC_LEN+1];  //用于存放转换后的数据 [长度]
    uint16_t i=0;
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf(buff, CAN_REC_LEN+1, fmt,  arg_ptr);//数据转换
    i=strlen(buff);//得出数据长度
    CAN_SendNormalData(canworkhcan,0x11,(uint8_t *)buff,i);//CAN发送数据函数（ID为0x11）
    va_end(arg_ptr);
}

//can总线通信
void CAN_senddata(uint16_t ID,uint8_t *pData,uint16_t  Len)
{
	 HAL_StatusTypeDef ret;//判断状态的枚举
	 ret = CAN_SendNormalData(canworkhcan,ID,pData,Len);
	 if(ret != HAL_OK)
	 {
		 printf("CAN_senddata err \r\n");
	 }
}
#endif
