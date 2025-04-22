#ifndef _ESP8266API_H_
#define _ESP8266API_H_
#include "esp8266_conf.h"
#if (ESP8266_ENBALE)


#include "esp8266.h"
void  Wifi_UserInit(void);
void  Wifi_UserProcess(void);
void  Wifi_UserGetUdpData(uint8_t LinkId,uint16_t DataLen,uint8_t *Data);
void  Wifi_UserGetTcpData(uint8_t LinkId,uint16_t DataLen,uint8_t *Data);

#endif
#endif
