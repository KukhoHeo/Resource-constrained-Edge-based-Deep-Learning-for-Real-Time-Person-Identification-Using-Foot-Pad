/*
 * FOOT_ADC.h
 *
 *  Created on: 2020. 9. 1.
 *      Author: Heo
 */

#ifndef APP_FOOT_ADC_H_
#define APP_FOOT_ADC_H_

#include <FOOT_HARD.h>
#include <FOOT_LED.h>
#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>



static GPIO_InitTypeDef GPIO_InitStruct;
ADC_HandleTypeDef AdcHandle;
uint16_t adcVal[2];
short adc_col,adc_row;
void HAL_ADC_MspInit(ADC_HandleTypeDef*);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef*);

void Error_Handler(void);
void Mux_Init();
void Mux_s0(short);
void Mux_s1(short);
void Mux_s2(short);
void Mux_s3(short);
void Mux_Open_by_Pin(int);
int Read_ADC();
void Read_Mux_Data(short);

void Read_Mux_Data_task1(void *);
void Read_Mux_Data_task2(void *);
void Read_Mux_Data_task3(void *);
#endif /* APP_FOOT_ADC_H_ */
