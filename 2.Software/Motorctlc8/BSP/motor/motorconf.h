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
#define MOTORGM37B_BIN1_Pin                   GPIO_PIN_14
#define MOTORGM37B_BIN1_GPIO_Port             GPIOB
#define MOTORGM37B_BIN2_Pin                   GPIO_PIN_15
#define MOTORGM37B_BIN2_GPIO_Port             GPIOB


#define MOTORGM37_HALLENABLE 1
#if MOTORGM37_HALLENABLE
#define MOTORGM37A_HALL_GPIO_Port            GPIOA
#define MOTORGM37A_HALL_Pin                  GPIO_PIN_7

#define MOTORGM37B_HALL_GPIO_Port            GPIOB
#define MOTORGM37B_HALL_Pin                  GPIO_PIN_1

#define MOTORGM37A_TIMER                       &htim3
#define MOTORGM37B_TIMER                       &htim3
#define MOTORGM37A_EXT_TIMER                 TIM3
#define MOTORGM37A_EXT_CH                    HAL_TIM_ACTIVE_CHANNEL_1

#define MOTORGM37B_EXT_TIMER                 TIM3
#define MOTORGM37B_EXT_CH                    HAL_TIM_ACTIVE_CHANNEL_3



#endif



#endif

#endif /* MOTORCONF_H_ */
