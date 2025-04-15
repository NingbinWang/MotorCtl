#ifndef _GPS_H_
#define _GPS_H_
#include "usart.h"
#include "nmea/nmea.h"


//#define __GPS_LOG_FILE    //定义这个宏，对SD卡上的gpslog.txt文件进行解码；不定义的话使用串口接收GPS信息解码

#if (GPS_ENBALE)
#define HALF_GPS_RBUFF_SIZE       (GPS_RBUFF_SIZE/2)    //串口接收缓冲区一半




void GPS_ProcessDMAIRQ(void);

void trace(const char *str, int str_size);
void error(const char *str, int str_size);
void gps_info(const char *str, int str_size);
void GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) ;


/**
  * @brief  GPS_SendDate
  * @param
  * @retval 无
  */

void GPS_SendDate(uint8_t *buffer, uint16_t length);

#endif


#endif
