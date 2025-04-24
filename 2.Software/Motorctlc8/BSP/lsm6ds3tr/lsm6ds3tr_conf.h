/*
 * lsm6ds3tr_conf.h
 *
 *  Created on: Apr 24, 2025
 *      Author: Administrator
 */

#ifndef _LSM6DS3TR_CONF_H_
#define _LSM6DS3TR_CONF_H_

#define LSM6DS3TR_ENABLE 1


#if LSM6DS3TR_ENABLE
#include "i2c.h"
#include "lsm6ds3tr.h"
#define LSM6DS3TR_I2C &hi2c1

void  lsm6dstr_Init(void);




#endif




#endif
