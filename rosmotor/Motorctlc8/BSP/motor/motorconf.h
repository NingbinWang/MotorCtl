/*
 * motorconf.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Administrator
 */

#ifndef MOTORCONF_H_
#define MOTORCONF_H_

#define MOTORGM37_ENABLE 1
#if MOTORGM37_ENABLE
#include "tim.h"
#include "gpio.h"
#define MOTORGM37A_PWM                        &htim2
#define MOTORGM37A_PWM_CH                     TIM_CHANNEL_1
#define MOTORGM37B_PWM                        &htim1
#define MOTORGM37B_PWM_CH                     TIM_CHANNEL_1

#define MOTORGM37A_AIN2_Pin                   GPIO_PIN_1
#define MOTORGM37A_AIN2_GPIO_Port             GPIOA
#define MOTORGM37A_AIN1_Pin                   GPIO_PIN_4
#define MOTORGM37A_AIN1_GPIO_Port             GPIOA
#define MOTORGM37A_BIN1_Pin                   GPIO_PIN_14
#define MOTORGM37A_BIN1_GPIO_Port             GPIOB
#define MOTORGM37A_BIN2_Pin                   GPIO_PIN_15
#define MOTORGM37A_BIN2_GPIO_Port             GPIOB

#endif

#endif /* MOTORCONF_H_ */
