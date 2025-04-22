/**@fn         esp8266Task
 * @brief      发送字符串后，空载一段时间
 * @param[in]  data     发送的消息
 * @return     成功返回 true  错误返回 false
 */
/*
void esp8266Task(void const * argument)
{
	osDelay(3000);
	esp8266_SendStringAndWait("AT\r\n",1000);
	esp8266_SetRfPower(82);
  Wifi_TcpIp_GetMultiConnection();
  Wifi_TcpIp_Close(0);
  Wifi_TcpIp_Close(1);
  Wifi_TcpIp_Close(2);
  Wifi_TcpIp_Close(3);
  Wifi_TcpIp_Close(4);
  Wifi_TcpIp_SetMultiConnection(true);
	Wifi_GetMode();
	Wifi_Station_DhcpIsEnable();
	Wifi_UserInit();




	//#######################
	while(1)
	{
		Wifi_GetMyIp();
    if((Wifi.Mode==WifiMode_SoftAp) || (Wifi.Mode==WifiMode_StationAndSoftAp))
      Wifi_SoftAp_GetConnectedDevices();
		Wifi_TcpIp_GetConnectionStatus();
    Wifi_RxClear();
		for(uint8_t i=0; i< 100; i++)
    {
      if( Wifi.GotNewData==true)
      {
        Wifi.GotNewData=false;
        for(uint8_t ii=0; ii<5 ; ii++)
        {
          if((strstr(Wifi.TcpIpConnections[ii].Type,"UDP")!=NULL) && (Wifi.RxDataConnectionNumber==Wifi.TcpIpConnections[ii].LinkId))
            Wifi_UserGetUdpData(Wifi.RxDataConnectionNumber,Wifi.RxDataLen,Wifi.RxBufferForData);
          if((strstr(Wifi.TcpIpConnections[ii].Type,"TCP")!=NULL) && (Wifi.RxDataConnectionNumber==Wifi.TcpIpConnections[ii].LinkId))
            Wifi_UserGetTcpData(Wifi.RxDataConnectionNumber,Wifi.RxDataLen,Wifi.RxBufferForData);
        }
      }
      osDelay(10);
    }
    Wifi_UserProcess();
	}
}


osSemaphoreDef(WifiSemHandle);
	WifiSemHandle = osSemaphoreCreate(osSemaphore(WifiSemHandle), 1);
	osThreadDef(WifiTaskName, esp8266Task, Priority, 0, _WIFI_TASK_SIZE);
	WifiTaskHandle = osThreadCreate(osThread(WifiTaskName), NULL);
*/
