/*
 * motor_gm37.c
 *
 *  Created on: Apr 15, 2025
 *      Author: Administrator
 */


#include "motorconf.h"
#if MOTORGM37_ENABLE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "motor_gm37.h"

/*
*********************************************************************************************************
*	函 数 名: MotorA_Forward
*	功能说明: 电机A正转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorGM37A_Forward(void)
{
    HAL_GPIO_WritePin(MOTORGM37A_AIN1_GPIO_Port, MOTORGM37A_AIN1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTORGM37A_AIN2_GPIO_Port, MOTORGM37A_AIN2_Pin, GPIO_PIN_SET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorA_Forward
*	功能说明: 电机A反转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorGM37A_Backward(void)
{
    HAL_GPIO_WritePin(MOTORGM37A_AIN1_GPIO_Port, MOTORGM37A_AIN1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(MOTORGM37A_AIN2_GPIO_Port, MOTORGM37A_AIN2_Pin, GPIO_PIN_RESET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorB_Forward
*	功能说明: 电机B正转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorGM37B_Forward(void)
{
    HAL_GPIO_WritePin(MOTORGM37A_BIN1_GPIO_Port, MOTORGM37A_BIN1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTORGM37A_BIN2_GPIO_Port, GPIO_PIN_3, GPIO_PIN_SET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorB_Backward
*	功能说明: 电机B反转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorGM37B_Backward(void)
{
    HAL_GPIO_WritePin(MOTORGM37A_BIN1_GPIO_Port, MOTORGM37A_BIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTORGM37A_BIN2_GPIO_Port, MOTORGM37A_BIN2_Pin, GPIO_PIN_RESET);
}

/*
*********************************************************************************************************
*	函 数 名: MotoASetSpeed
*	功能说明: A电机的占空比设置
*	形    参: _direction ：运动方向 00表示正转 01表示反转 _speed：占空比值 范围0~2000
*	返 回 值: 无
*********************************************************************************************************
*/

void MotorGM37ASetSpeed(int8_t _direction, int16_t _speed)
{
    if(_direction == 0)

    {

    	MotorGM37A_Forward();
        __HAL_TIM_SET_COMPARE(MOTORGM37A_PWM, MOTORGM37A_PWM_CH, abs(_speed));
    }
    else
    {
    	MotorGM37A_Backward();
        __HAL_TIM_SET_COMPARE(MOTORGM37A_PWM, MOTORGM37A_PWM_CH, abs(_speed));
    }

    HAL_TIM_PWM_Start(MOTORGM37A_PWM, MOTORGM37A_PWM_CH);
}

/*
*********************************************************************************************************
*	函 数 名: MotoBSetSpeed
*	功能说明: B电机的占空比设置
*	形    参: _direction ：运动方向 00表示正转 01表示反转 _speed：占空比值 范围0~2000
*	返 回 值: 无
*********************************************************************************************************
*/

void MotorGM37BSetSpeed(int8_t _direction, int16_t _speed)
{
    if(_direction == 0)
    {
    	MotorGM37B_Forward();

        __HAL_TIM_SET_COMPARE(MOTORGM37B_PWM, MOTORGM37B_PWM_CH, abs(_speed));

    }
    else
    {
    	MotorGM37B_Backward();
        __HAL_TIM_SET_COMPARE(MOTORGM37B_PWM, MOTORGM37B_PWM_CH, abs(_speed));
    }
    HAL_TIM_PWM_Start(MOTORGM37B_PWM, MOTORGM37B_PWM_CH);

}

void MotorGM37APWMStart()
{
    HAL_TIM_PWM_Start(MOTORGM37A_PWM, MOTORGM37A_PWM_CH);
}


void MotorGM37BPWMStart()
{
    HAL_TIM_PWM_Start(MOTORGM37B_PWM, MOTORGM37B_PWM_CH);
}


#endif
