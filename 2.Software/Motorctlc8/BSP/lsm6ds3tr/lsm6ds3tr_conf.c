/*
 * lsm6ds3tr_conf.c
 *
 *  Created on: Apr 24, 2025
 *      Author: Administrator
 */

#include "lsm6ds3tr_conf.h"
#if LSM6DS3TR_ENABLE
#include "cmsis_os.h"
#include <stdlib.h>


stmdev_ctx_t* lsm6dstr_ctx;
#define lsm6dstr_timeout  100

int32_t  lsm6dstr_read_reg(void *handle, uint8_t reg, uint8_t * data, uint16_t len)
{
	 HAL_I2C_Mem_Read(handle, LSM6DS3TR_C_I2C_ADD_L, reg, 1, data, len, lsm6dstr_timeout);
	 return 0;
}

int32_t  lsm6dstr_write_reg(void *handle, uint8_t reg, const uint8_t * data, uint16_t len)
{
	 HAL_I2C_Mem_Write(handle, LSM6DS3TR_C_I2C_ADD_L, reg, 1, (uint8_t *)data, len, lsm6dstr_timeout);
	 return 0;
}

void  lsm6dstr_delay(uint32_t millisec)
{
	osDelay(millisec);
}


void  lsm6dstr_Init(void)
{
	lsm6dstr_ctx = (stmdev_ctx_t*)malloc(sizeof(stmdev_ctx_t));
	lsm6dstr_ctx->write_reg = lsm6dstr_write_reg;
	lsm6dstr_ctx->read_reg = lsm6dstr_read_reg;
	lsm6dstr_ctx->mdelay = lsm6dstr_delay;
	lsm6dstr_ctx->handle = (void *)LSM6DS3TR_I2C;
}





#endif
