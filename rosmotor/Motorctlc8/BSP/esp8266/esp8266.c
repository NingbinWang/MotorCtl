#include "esp8266_conf.h"
#if (ESP8266_ENBALE)

#include "esp8266.h"
#include "esp8266test.h"

osThreadId 		WifiTaskHandle;
osSemaphoreId 	WifiSemHandle;


WIFI_T	Wifi;

/**@fn         esp8266_SendRaw
 * @brief      send message
 * @param[in]  data     data of message
 * @param[in]  len      length of message
 * @return     sucess return true  ,fail return  false
 */
bool esp8266_SendRaw(uint8_t *data,uint16_t len)
{
	if(len <= _WIFI_TX_SIZE)
	{
		memcpy(Wifi.TxBuffer,data,len);
		if(HAL_UART_Transmit(&_WIFI_USART,data,len,100) == HAL_OK)
			return true;
		else
			return false;
	}
	else
		return false;
}

/**@fn         esp8266_SendString
 * @brief      send a string.
 * @param[in]  data     string
 * @return     sucess return true  ,fail return  false
 */
bool esp8266_SendString(char *data)
{
	return esp8266_SendRaw((uint8_t*)data,strlen(data));
}

/**@fn         esp8266_SendStringAndWait
 * @brief      send a string. and then wait for some time
 * @param[in]  data        string
 * @param[in]  DelayMs     wait time
 * @return    sucess return true  ,fail return  false
 */
bool esp8266_SendStringAndWait(char *data,uint16_t DelayMs)
{
	if(esp8266_SendRaw((uint8_t*)data,strlen(data))==false)
		return false;
	osDelay(DelayMs);
	return true;
}

/**@fn         esp8266_WaitForString
 * @brief      wait for astring
 * @param[in]  TimeOut_ms
 * @param[out] result
 * @return     sucess return true  ,fail return  false
 */
bool esp8266_WaitForString(uint32_t TimeOut_ms,uint8_t *result,uint8_t CountOfParameter,...)
{

	if(result == NULL)
		return false;
	if(CountOfParameter == 0)
		return false;
	*result=0;
    va_list tag;
	va_start (tag,CountOfParameter);
	char *arg[CountOfParameter];
	for(uint8_t i=0; i<CountOfParameter ; i++)
		arg[i] = va_arg (tag, char *);
    va_end (tag);
	for(uint32_t t=0 ; t<TimeOut_ms ; t+=50)
	{
		osDelay(50);
		for(uint8_t	mx=0 ; mx<CountOfParameter ; mx++)
		{
			if(strstr((char*)Wifi.RxBuffer,arg[mx])!=NULL)
			{
				*result = mx+1;
				return true;
			}
		}
	}
	// timeout
	return false;

}

/**@fn         esp8266_ReturnString
 * @brief      return string
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	esp8266_ReturnString(char *result,uint8_t WantWhichOne,char *SplitterChars)
{
	if(result == NULL)
		return false;
	if(WantWhichOne==0)
		return false;

	char *str = (char*)Wifi.RxBuffer;


	str = strtok (str,SplitterChars);
	if(str == NULL)
	{
		strcpy(result,"");
		return false;
	}
	while (str != NULL)
  {
    str = strtok (NULL,SplitterChars);
		if(str != NULL)
			WantWhichOne--;
		if(WantWhichOne==0)
		{
			strcpy(result,str);
			return true;
		}
  }
	strcpy(result,"");
	return false;
}

/**@fn         esp8266_ReturnStrings
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool esp8266_ReturnStrings(char *InputString,char *SplitterChars,uint8_t CountOfParameter,...)
{
	if(CountOfParameter == 0)
		return false;
	va_list tag;
	va_start (tag,CountOfParameter);
	char *arg[CountOfParameter];
	for(uint8_t i=0; i<CountOfParameter ; i++)
		arg[i] = va_arg (tag, char *);
  va_end (tag);

	char *str;
	str = strtok (InputString,SplitterChars);
	if(str == NULL)
		return false;
	uint8_t i=0;
	while (str != NULL)
  {
    str = strtok (NULL,SplitterChars);
		if(str != NULL)
			CountOfParameter--;
		strcpy(arg[i],str);
		i++;
		if(CountOfParameter==0)
		{
			return true;
		}
  }
	return false;

}

/**@fn         esp8266_ReturnInteger
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	esp8266_ReturnInteger(int32_t	*result,uint8_t WantWhichOne,char *SplitterChars)
{
	if((char*)Wifi.RxBuffer == NULL)
		return false;
	if(esp8266_ReturnString((char*)Wifi.RxBuffer,WantWhichOne,SplitterChars)==false)
		return false;
	*result = atoi((char*)Wifi.RxBuffer);
	return true;
}

/**@fn         esp8266_ReturnString_ReturnFloat
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	esp8266_ReturnString_ReturnFloat(float	*result,uint8_t WantWhichOne,char *SplitterChars)
{
	if((char*)Wifi.RxBuffer == NULL)
		return false;
	if(esp8266_ReturnString((char*)Wifi.RxBuffer,WantWhichOne,SplitterChars)==false)
		return false;
	*result = atof((char*)Wifi.RxBuffer);
	return true;
}

/**@fn         esp8266_RemoveChar
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
void esp8266_RemoveChar(char *str, char garbage)
{
	char *src, *dst;
  for (src = dst = str; *src != '\0'; src++)
	{
		*dst = *src;
		if (*dst != garbage)
			dst++;
  }
  *dst = '\0';
}

/**@fn         esp8266_RxClear
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
void	esp8266_RxClear(void)
{
	memset(Wifi.RxBuffer,0,_WIFI_RX_SIZE);
	Wifi.RxIndex=0;
    HAL_UART_Receive_IT(&_WIFI_USART,&Wifi.usartBuff,1);
}

/**@fn         esp8266_TxClear
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
void	esp8266_TxClear(void)
{
	memset(Wifi.TxBuffer,0,_WIFI_TX_SIZE);
}

/**@fn         esp8266_RxCallBack
 * @brief
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
void	esp8266_RxCallBack(void)
{
  //+++ at command buffer
  if(Wifi.RxIsData==false)
  {
    Wifi.RxBuffer[Wifi.RxIndex] = Wifi.usartBuff;
    if(Wifi.RxIndex < _WIFI_RX_SIZE)
      Wifi.RxIndex++;
  }
  //--- at command buffer
  //+++  data buffer
  else
  {
    if( HAL_GetTick()-Wifi.RxDataLastTime > 50)
      Wifi.RxIsData=false;
    //+++ Calculate Data len after +IPD
    if(Wifi.RxDataLen==0)
    {
      //+++ Calculate Data len after +IPD ++++++ Multi Connection OFF
      if (Wifi.TcpIpMultiConnection==false)
      {
        Wifi.RxBufferForDataTmp[Wifi.RxIndexForDataTmp] = Wifi.usartBuff;
        Wifi.RxIndexForDataTmp++;
        if(Wifi.RxBufferForDataTmp[Wifi.RxIndexForDataTmp-1]==':')
        {
          Wifi.RxDataConnectionNumber=0;
          Wifi.RxDataLen=atoi((char*)&Wifi.RxBufferForDataTmp[1]);
        }
      }
      //--- Calculate Data len after +IPD ++++++ Multi Connection OFF
      //+++ Calculate Data len after +IPD ++++++ Multi Connection ON
      else
      {
        Wifi.RxBufferForDataTmp[Wifi.RxIndexForDataTmp] = Wifi.usartBuff;
        Wifi.RxIndexForDataTmp++;
        if(Wifi.RxBufferForDataTmp[2]==',')
        {
          Wifi.RxDataConnectionNumber=Wifi.RxBufferForDataTmp[1]-48;
        }
        if((Wifi.RxIndexForDataTmp>3) && (Wifi.RxBufferForDataTmp[Wifi.RxIndexForDataTmp-1]==':'))
          Wifi.RxDataLen=atoi((char*)&Wifi.RxBufferForDataTmp[3]);
      }
      //--- Calculate Data len after +IPD ++++++ Multi Connection ON
    }
    //--- Calculate Data len after +IPD
    //+++ Fill Data Buffer
    else
    {
      Wifi.RxBufferForData[Wifi.RxIndexForData] = Wifi.usartBuff;
      if(Wifi.RxIndexForData < _WIFI_RX_FOR_DATA_SIZE)
        Wifi.RxIndexForData++;
      if( Wifi.RxIndexForData>= Wifi.RxDataLen)
      {
        Wifi.RxIsData=false;
        Wifi.GotNewData=true;
      }
    }
    //--- Fill Data Buffer
  }
  //--- data buffer
	HAL_UART_Receive_IT(&_WIFI_USART,&Wifi.usartBuff,1);
  //+++ check +IPD in At command buffer
  if(Wifi.RxIndex>4)
  {
    if( (Wifi.RxBuffer[Wifi.RxIndex-4]=='+') && (Wifi.RxBuffer[Wifi.RxIndex-3]=='I') && (Wifi.RxBuffer[Wifi.RxIndex-2]=='P') && (Wifi.RxBuffer[Wifi.RxIndex-1]=='D'))
    {
      memset(Wifi.RxBufferForDataTmp,0,sizeof(Wifi.RxBufferForDataTmp));
      Wifi.RxBuffer[Wifi.RxIndex-4]=0;
      Wifi.RxBuffer[Wifi.RxIndex-3]=0;
      Wifi.RxBuffer[Wifi.RxIndex-2]=0;
      Wifi.RxBuffer[Wifi.RxIndex-1]=0;
      Wifi.RxIndex-=4;
      Wifi.RxIndexForData=0;
      Wifi.RxIndexForDataTmp=0;
      Wifi.RxIsData=true;
      Wifi.RxDataLen=0;
      Wifi.RxDataLastTime = HAL_GetTick();
    }
  }
  //--- check +IPD in At command buffer
}




/**@fn         esp8266_Init
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
void	esp8266_Init(osPriority	Priority)
{
	HAL_UART_Receive_IT(&_WIFI_USART,&Wifi.usartBuff,1);
	esp8266_RxClear();
	esp8266_TxClear();
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	esp8266_Restart(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		esp8266_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+RST\r\n");
		if(esp8266_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(esp8266_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         esp8266_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	esp8266_DeepSleep(uint16_t DelayMs)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+GSLP=%d\r\n",DelayMs);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}


/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_FactoryReset(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+RESTORE\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}


/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_Update(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIUPDATE\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(1000*60*5,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_SetRfPower(uint8_t Power_0_to_82)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+RFPOWER=%d\r\n",Power_0_to_82);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_SetMode(WIFIMODE_E	WifiMode_)
{
	//osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWMODE_CUR=%d\r\n",WifiMode_);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		Wifi.Mode = WifiMode_;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_GetMode(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWMODE_CUR?\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		if(Wifi_ReturnInteger((int32_t*)&result,1,":"))
			Wifi.Mode = (WIFIMODE_E)result ;
		else
			Wifi.Mode = WifiMode_Error;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_GetMyIp(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIFSR\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		sscanf((char*)Wifi.RxBuffer,"AT+CIFSR\r\r\n+CIFSR:APIP,\"%[^\"]",Wifi.MyIP);
    sscanf((char*)Wifi.RxBuffer,"AT+CIFSR\r\r\n+CIFSR:STAIP,\"%[^\"]",Wifi.MyIP);


    Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIPSTA?\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;

    char *str=strstr((char*)Wifi.RxBuffer,"gateway:");
    if(str==NULL)
      break;
    if(Wifi_ReturnStrings(str,"\"",1,Wifi.MyGateWay)==false)
      break;

		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_Station_ConnectToAp(char *SSID,char *Pass,char *MAC)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		if(MAC==NULL)
			sprintf((char*)Wifi.TxBuffer,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",SSID,Pass);
		else
			sprintf((char*)Wifi.TxBuffer,"AT+CWJAP_CUR=\"%s\",\"%s\",\"%s\"\r\n",SSID,Pass,MAC);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_VERYHIGH,&result,3,"\r\nOK\r\n","\r\nERROR\r\n","\r\nFAIL\r\n")==false)
			break;
		if( result > 1)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_Station_Disconnect(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWQAP\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_Station_DhcpEnable(bool Enable)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWDHCP_CUR=1,%d\r\n",Enable);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		Wifi.StationDhcp=Enable;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_Station_DhcpIsEnable(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWDHCP_CUR?\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		if(Wifi_ReturnInteger((int32_t*)&result,1,":")==false)
			break;
		switch(result)
		{
			case 0:
				Wifi.StationDhcp=false;
				Wifi.SoftApDhcp=false;
			break;
			case 1:
				Wifi.StationDhcp=false;
				Wifi.SoftApDhcp=true;
			break;
			case 2:
				Wifi.StationDhcp=true;
				Wifi.SoftApDhcp=false;
			break;
			case 3:
				Wifi.StationDhcp=true;
				Wifi.SoftApDhcp=true;
			break;
		}
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool	Wifi_Station_SetIp(char *IP,char *GateWay,char *NetMask)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIPSTA_CUR=\"%s\",\"%s\",\"%s\"\r\n",IP,GateWay,NetMask);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		Wifi.StationDhcp=false;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_SoftAp_GetConnectedDevices(void)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWLIF\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		Wifi_RemoveChar((char*)Wifi.RxBuffer,'\r');
    Wifi_ReturnStrings((char*)Wifi.RxBuffer,"\n,",10,Wifi.SoftApConnectedDevicesIp[0],Wifi.SoftApConnectedDevicesMac[0],Wifi.SoftApConnectedDevicesIp[1],Wifi.SoftApConnectedDevicesMac[1],Wifi.SoftApConnectedDevicesIp[2],Wifi.SoftApConnectedDevicesMac[2],Wifi.SoftApConnectedDevicesIp[3],Wifi.SoftApConnectedDevicesMac[3],Wifi.SoftApConnectedDevicesIp[4],Wifi.SoftApConnectedDevicesMac[4]);
		for(uint8_t i=0 ; i<6 ; i++)
    {
      if( (Wifi.SoftApConnectedDevicesIp[i][0]<'0') || (Wifi.SoftApConnectedDevicesIp[i][0]>'9'))
        Wifi.SoftApConnectedDevicesIp[i][0]=0;
      if( (Wifi.SoftApConnectedDevicesMac[i][0]<'0') || (Wifi.SoftApConnectedDevicesMac[i][0]>'9'))
        Wifi.SoftApConnectedDevicesMac[i][0]=0;
    }

		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_SoftAp_Create(char *SSID,char *password,uint8_t channel,WIFIEPT_E WifiEncryptionType,uint8_t MaxConnections_1_to_4,bool HiddenSSID)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CWSAP=\"%s\",\"%s\",%d,%d,%d,%d\r\n",SSID,password,channel,WifiEncryptionType,MaxConnections_1_to_4,HiddenSSID);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_GetConnectionStatus(void)
{
	osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIPSTATUS\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;


		char *str = strstr((char*)Wifi.RxBuffer,"\nSTATUS:");
    if(str==NULL)
      break;
    str = strchr(str,':');
    str++;
    for(uint8_t i=0 ; i<5 ;i++)
      Wifi.TcpIpConnections[i].status=(WIFIEPT_E)atoi(str);
    str = strstr((char*)Wifi.RxBuffer,"+CIPSTATUS:");
    for(uint8_t i=0 ; i<5 ;i++)
    {
      sscanf(str,"+CIPSTATUS:%d,\"%3s\",\"%[^\"]\",%d,%d,%d",(int*)&Wifi.TcpIpConnections[i].LinkId,Wifi.TcpIpConnections[i].Type,Wifi.TcpIpConnections[i].RemoteIp,(int*)&Wifi.TcpIpConnections[i].RemotePort,(int*)&Wifi.TcpIpConnections[i].LocalPort,(int*)&Wifi.TcpIpConnections[i].RunAsServer);
      str++;
      str = strstr(str,"+CIPSTATUS:");
      if(str==NULL)
        break;
    }
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_Ping(char *PingTo)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+PING=\"%s\"\r\n",PingTo);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_MED,&result,3,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
    if(Wifi_ReturnInteger((int32_t*)&Wifi.TcpIpPingAnswer,2,"+")==false)
      break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_SetMultiConnection(bool EnableMultiConnections)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIPMUX=%d\r\n",EnableMultiConnections);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
    Wifi.TcpIpMultiConnection=EnableMultiConnections;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_GetMultiConnection(void)
{

  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
		sprintf((char*)Wifi.TxBuffer,"AT+CIPMUX?\r\n");
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
    if(Wifi_ReturnInteger((int32_t*)&result,1,":")==false)
      break;
    Wifi.TcpIpMultiConnection=(bool)result;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_StartTcpConnection(uint8_t LinkId,char *RemoteIp,uint16_t RemotePort,uint16_t TimeOut)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
    Wifi_RxClear();
    sprintf((char*)Wifi.TxBuffer,"AT+CIPSERVER=1,%d\r\n",RemotePort);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		Wifi_RxClear();
    if(Wifi.TcpIpMultiConnection==false)
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSTART=\"TCP\",\"%s\",%d,%d\r\n",RemoteIp,RemotePort,TimeOut);
    else
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSTART=%d,\"TCP\",\"%s\",%d,%d\r\n",LinkId,RemoteIp,RemotePort,TimeOut);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_HIGH,&result,3,"OK","CONNECT","ERROR")==false)
			break;
		if(result == 3)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_StartUdpConnection(uint8_t LinkId,char *RemoteIp,uint16_t RemotePort,uint16_t LocalPort)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
    if(Wifi.TcpIpMultiConnection==false)
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSTART=\"UDP\",\"%s\",%d,%d\r\n",RemoteIp,RemotePort,LocalPort);
    else
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSTART=%d,\"UDP\",\"%s\",%d,%d\r\n",LinkId,RemoteIp,RemotePort,LocalPort);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_HIGH,&result,3,"OK","ALREADY","ERROR")==false)
			break;
		if(result == 3)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_Close(uint8_t LinkId)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
    if(Wifi.TcpIpMultiConnection==false)
      sprintf((char*)Wifi.TxBuffer,"AT+CIPCLOSE\r\n");
    else
      sprintf((char*)Wifi.TxBuffer,"AT+CIPCLOSE=%d\r\n",LinkId);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_SetEnableTcpServer(uint16_t PortNumber)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
    if(Wifi.TcpIpMultiConnection==false)
    {
      sprintf((char*)Wifi.TxBuffer,"AT+CIPMUX=1\r\n");
      if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
        break;
      if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
        break;
      Wifi.TcpIpMultiConnection=true;
      Wifi_RxClear();
    }
    else
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSERVER=1,%d\r\n",PortNumber);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_SetDisableTcpServer(uint16_t PortNumber)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
    sprintf((char*)Wifi.TxBuffer,"AT+CIPSERVER=0,%d\r\n",PortNumber);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		if(result == 2)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_SendDataUdp(uint8_t LinkId,uint16_t dataLen,uint8_t *data)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		Wifi_RxClear();
    if(Wifi.TcpIpMultiConnection==false)
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSERVER=0\r\n");
    else
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSEND=%d,%d\r\n",LinkId,dataLen);
		if(Wifi_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,">","ERROR")==false)
			break;
		if(result == 2)
			break;
    Wifi_RxClear();
    Wifi_SendRaw(data,dataLen);
    if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;

}

/**@fn         Wifi_ReturnStrings
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
bool  Wifi_TcpIp_SendDataTcp(uint8_t LinkId,uint16_t dataLen,uint8_t *data)
{
  osSemaphoreWait(WifiSemHandle,osWaitForever);
	uint8_t result;
	bool		returnVal=false;
	do
	{
		esp8266_RxClear();
    if(Wifi.TcpIpMultiConnection==false)
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSENDBUF=%d\r\n",dataLen);
    else
      sprintf((char*)Wifi.TxBuffer,"AT+CIPSENDBUF=%d,%d\r\n",LinkId,dataLen);
		if(esp8266_SendString((char*)Wifi.TxBuffer)==false)
			break;
		if(esp8266_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
			if(esp8266_WaitForString(_WIFI_WAIT_TIME_LOW,&result,3,">","ERROR","busy")==false)
			break;
		if(result > 1)
			break;
    esp8266_RxClear();
    Wifi_SendRaw(data,dataLen);
    if(Wifi_WaitForString(_WIFI_WAIT_TIME_LOW,&result,2,"OK","ERROR")==false)
			break;
		returnVal=true;
	}while(0);
	osSemaphoreRelease(WifiSemHandle);
	return returnVal;
}
























#endif
