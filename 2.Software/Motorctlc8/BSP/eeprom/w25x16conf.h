#ifndef _W25X16_CONFIG_H_
#define _W25X16_CONFIG_H_

#define W25X16_ENABLE 0

#if (W25X16_ENABLE)
#include "spi.h"
#include "gpio.h"

#define	SPI_FLASH_CS_0      HAL_GPIO_WritePin(flash_cs_GPIO_Port, flash_cs_Pin, GPIO_PIN_RESET)
#define SPI_FLASH_CS_1      HAL_GPIO_WritePin(flash_cs_GPIO_Port, flash_cs_Pin, GPIO_PIN_SET)

#define W25X16hspi &hspi1

#endif
#endif
