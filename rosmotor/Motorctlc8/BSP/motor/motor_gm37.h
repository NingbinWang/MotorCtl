/*
 * motor_gm37.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Administrator
 */

#ifndef MOTOR_GM37_H_
#define MOTOR_GM37_H_
#include <stdint.h>
void MotorGM37ASetSpeed(int8_t _direction, int16_t _speed);
void MotorGM37BSetSpeed(int8_t _direction, int16_t _speed);
void MotorGM37APWMStart();
void MotorGM37BPWMStart();

#endif /* MOTOR_MOTOR_GM37_H_ */
