/*
 * motor_gm37hall.c
 *
 *  Created on: Apr 19, 2025
 *      Author: Administrator
 */
#include "motorconf.h"
#if MOTORGM37_HALLENABLE
#include "motor_gm37hallencoder.h"
Encoder_t gEncoderValue;
static int16_t sEncodeValue[4] = {0};
int16_t gWheelSpeed[2] = {0, 0};//数组中的每一个元素分别代表一个车轮,用于存放车轮转速
int16_t SendWheelSpeed[2] = {0, 0};
/*
*********************************************************************************************************
*	函 数 名: GetSpeedInfo
*	功能说明: 把编码器测得的值转化为小车实际速度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void GetSpeedInfo(int16_t *_speed, uint16_t _EncoderDirection1, uint16_t _EncoderDirection2, uint16_t _FeedbackDir)
{
    uint16_t _encoder = 0;

    _encoder = _EncoderDirection1 > _EncoderDirection2 ? _EncoderDirection1 : _EncoderDirection2;

		if(_EncoderDirection1>_EncoderDirection2)
		{
			*_speed = _encoder * 20 * 3.1416 * 65  / 330; //65-轮胎直径 330-编码器线数 20-编码器获取方波的频率
		}
		else
			*_speed = -1 * _encoder * 20 * 3.1416 * 65  / 330; //65-轮胎直径 330-编码器线数 20-编码器获取方波的频率
}

/*
*********************************************************************************************************
*	函 数 名: HAL_TIM_IC_CaptureCallback
*	功能说明: TIM_IC回调函数
*	形    参: htim
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)/*输入捕获中断回调函数*/
{
    if(htim->Instance == MOTORGM37A_EXT_TIMER)/*判断定时器*/
    {
         if(htim->Channel == MOTORGM37A_EXT_CH)/*判断通道*/
        {
            if(HAL_GPIO_ReadPin(MOTORGM37A_HALL_GPIO_Port, MOTORGM37A_HALL_Pin) == 1)
            {
            	gEncoderValue.MotorA_EncoderValue.dir = 1;
            	sEncodeValue[0] ++;

            }
            else if(HAL_GPIO_ReadPin(MOTORGM37A_HALL_GPIO_Port, MOTORGM37A_HALL_Pin) == 0)
            {
            	gEncoderValue.MotorA_EncoderValue.dir = 0;
                sEncodeValue[1] ++;

            }
          //  printf("d: %d\n", gEncoderValue.MotorA_EncoderValue.dir);
        }
    }

    if(htim->Instance == MOTORGM37B_EXT_TIMER)/*判断定时器*/
    {
         if(htim->Channel == MOTORGM37B_EXT_CH)/*判断通道*/
        {
            if(HAL_GPIO_ReadPin(MOTORGM37B_HALL_GPIO_Port, MOTORGM37B_HALL_Pin) == 1)
            {
            	gEncoderValue.MotorB_EncoderValue.dir = 1;
            	sEncodeValue[2] ++;

            }
            else if(HAL_GPIO_ReadPin(MOTORGM37B_HALL_GPIO_Port, MOTORGM37B_HALL_Pin) == 0)
            {
            	gEncoderValue.MotorB_EncoderValue.dir = 0;
                sEncodeValue[3] ++;

            }
          //  printf("d: %d\n", gEncoderValue.MotorA_EncoderValue.dir);
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: HAL_TIM_PeriodElapsedCallback
*	功能说明: 定时器5更新中断
*	形    参: htim
*	返 回 值: 无
*********************************************************************************************************
*/

void MotorGM37_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Instance == MOTORGM37A_EXT_TIMER)/*判断定时器*/
	 {
		 if(htim->Channel == MOTORGM37A_EXT_CH)/*判断通道*/
		 {
			 GetSpeedInfo(gWheelSpeed, sEncodeValue[0], sEncodeValue[1], gEncoderValue.MotorA_EncoderValue.dir);
		 }
	 }
	 if(htim->Instance == MOTORGM37B_EXT_TIMER)/*判断定时器*/
	 {
	     if(htim->Channel == MOTORGM37B_EXT_CH)/*判断通道*/
	     {
               GetSpeedInfo(gWheelSpeed + 1, sEncodeValue[2], sEncodeValue[3], gEncoderValue.MotorB_EncoderValue.dir);
	     }
	 }


    for(int i = 0; i < 8; i ++)//每次更新中断时，清空：s_EncodeValue[i]
    {
        sEncodeValue[i] = 0;

    }
    SendWheelSpeed[0] = -gWheelSpeed[0];
	SendWheelSpeed[1] = -gWheelSpeed[1];
    //KDCOM_SendData(KDCOM_SetData(&kdrobot_channel_1,0,SendWheelSpeed[0],_sMotorA_Speed_M),sizeof(kdrobot_channel_1));
	//KDCOM_SendData(KDCOM_SetData(&kdrobot_channel_2,1,SendWheelSpeed[1],_sMotorB_Speed_M),sizeof(kdrobot_channel_2));

}






#endif

