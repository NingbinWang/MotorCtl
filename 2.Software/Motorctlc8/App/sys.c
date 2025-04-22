#include <stdio.h>
#include "config.h"
// 重定向printf start
//_write函數在syscalls.c中， 使用__weak定义可以直接在其他文件中定义_write函數
__attribute__((weak)) int _write(int file, char *ptr, int len)
{
	 if(HAL_UART_Transmit(&PRINTFuartbus,(uint8_t *)ptr,len,0xffff) != HAL_OK)
	 {
		 Error_Handler();
	 }
	 return len;
}
