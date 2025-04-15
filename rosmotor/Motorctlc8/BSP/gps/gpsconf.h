#ifndef _GPSCONF_H_
#define _GPSCONF_H_


#define GPS_ENBALE 0

#if (GPS_ENBALE)
#include "usart.h"
#include "dma.h"
#include <stdbool.h>
//config
#define __GPS_DEBUG  1

//
#define		GPS_USART									huart3



//DMA
#define GPS_USART_DMA             hdma_usart3_rx
#define GPS_RBUFF_SIZE            512                   //串口接收缓冲区大小



typedef struct _USART_DMA_ {
	bool 	 recv_flag;				// 接收完成标志
	uint8_t  send_buf[GPS_RBUFF_SIZE];		// 发送缓冲区
	uint8_t  recv_buf[GPS_RBUFF_SIZE];		// 接收缓冲区
	uint8_t  dma_buf[GPS_RBUFF_SIZE];		// dma缓冲区
	uint16_t recv_len;					// 接收数据的长度
}GPS_DMA_T;




/* GPS接口配置 使用不同的串口时，要修改对应的接口 */


//DMA
#define GPS_USART_DMA_STREAM            DMA1_Stream1
#define GPS_DMA_IRQn                     DMA1_Stream1_IRQn         //GPS中断源



/* 外设标志 */


/* 中断函数 */
#define GPS_DMA_IRQHANDLER           DMA1_Stream1_IRQHandler   //GPS使用的DMA中断服务函数









#endif

#endif
