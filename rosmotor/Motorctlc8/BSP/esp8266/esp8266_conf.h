#ifndef _ESP8266_CONF_H_
#define _ESP8266_CONF_H_

#include "usart.h"


#define ESP8266_ENBALE  0

#if (ESP8266_ENBALE)
#define		_WIFI_USART									huart2
#define		_WIFI_RX_SIZE								512
#define		_WIFI_RX_FOR_DATA_SIZE			            1024
#define		_WIFI_TX_SIZE								256


#define		_WIFI_WAIT_TIME_LOW					1000
#define		_WIFI_WAIT_TIME_MED					10000
#define		_WIFI_WAIT_TIME_HIGH				25000
#define		_WIFI_WAIT_TIME_VERYHIGH		    60000

#endif



#endif
