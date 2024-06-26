/*
 * foot_led.c
 *
 *  Created on: 2020. 6. 2.
 *      Author: Heo
 */
#ifndef APP_FOOT_LED_H_
#define APP_FOOT_LED_H_

#include <FOOT_HARD.h>
#include <ubinos.h>

#include <string.h>
#include <stdio.h>

static GPIO_InitTypeDef GPIO_InitStruct;
void Print_Foot_Pad();
void Init_Foot_Pad();
void Led_Init();
void Led_Turn_on_1s(char *);
void Led_Turn_on(char *);
void Led_Turn_off(char *);
#endif /* APP_FOOT_LED_H_ */
