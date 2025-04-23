/*
 * motor_gm37hall.h
 *
 *  Created on: Apr 19, 2025
 *      Author: Administrator
 */

#ifndef MOTOR_GM37HALLENCODER_H_
#define MOTOR_GM37HALLENCODER_H_
#include <stdint.h>
#include "tim.h"
typedef struct
{
    uint8_t  dir;
    uint16_t speed;

}CarSpeedInfo_t;

typedef struct {

    CarSpeedInfo_t MotorA_EncoderValue;
    CarSpeedInfo_t MotorB_EncoderValue;
}Encoder_t;

void MotorGM37_UpdateSpeed(void);
void MotorGM37_IC_CaptureCallback(TIM_HandleTypeDef *htim);/*输入捕获中断回调函数*/
int16_t MotorGM37_GetSendWheelSpeed(int index);
int16_t MotorGM37_GetEncodeValue(int index);
void MotorGM37_Hallencoder_Start();
//extern Encoder_t gEncoderValue;
//extern int16_t gWheelSpeed[4];
//extern int16_t SendWheelSpeed[4];

#define   REDUCTION_RATIO    30//电机减速比
#endif /* MOTOR_GM37HALL_H_ */
