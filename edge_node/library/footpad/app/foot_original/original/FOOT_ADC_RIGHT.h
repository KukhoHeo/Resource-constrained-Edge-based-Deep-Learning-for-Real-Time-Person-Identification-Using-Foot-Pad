/*
 * FOOT_ADC_RIGHT.h
 *
 *  Created on: 2020. 6. 10.
 *      Author: Heo
 */

#ifndef APP_FOOT_ADC_RIGHT_H_
#define APP_FOOT_ADC_RIGHT_H_

#include <FOOT_HARD.h>
//GPIO Init Structure
static GPIO_InitTypeDef GPIO_InitStruct;

/* ADC handler declaration */
ADC_HandleTypeDef AdcHandle_Right;

static void Error_Handler_Right(void) {
	/* Turn LED3 on */
	Led_Turn_on("GREEN");
}

static void Mux_Right_Init() {
	//RIGHT MUX Init
	Mux_Right_S0_ENABLE();
	Mux_Right_S1_ENABLE();
	Mux_Right_S2_ENABLE();
	Mux_Right_S3_ENABLE();
	Mux_Right_Mux_1_EN_ENABLE();
	Mux_Right_Mux_2_EN_ENABLE();
	Mux_Right_Mux_3_EN_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = Mux_Right_S0_PIN;
	HAL_GPIO_Init(Mux_Right_S0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_S1_PIN;
	HAL_GPIO_Init(Mux_Right_S1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_S2_PIN;
	HAL_GPIO_Init(Mux_Right_S2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_S3_PIN;
	HAL_GPIO_Init(Mux_Right_S3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_Mux_1_EN_PIN;
	HAL_GPIO_Init(Mux_Right_Mux_1_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_Mux_2_EN_PIN;
	HAL_GPIO_Init(Mux_Right_Mux_2_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Right_Mux_3_EN_PIN;
	HAL_GPIO_Init(Mux_Right_Mux_3_EN_PORT, &GPIO_InitStruct);
	//RIGHT ADC INIT Start
	ADC_ChannelConfTypeDef sConfig_Right;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle_Right.Instance = ADCx_Right_INSTANCE;

	AdcHandle_Right.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle_Right.Init.Resolution = ADC_RESOLUTION_12B;
	AdcHandle_Right.Init.ScanConvMode = DISABLE;
	AdcHandle_Right.Init.ContinuousConvMode = DISABLE;
	AdcHandle_Right.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle_Right.Init.NbrOfDiscConversion = 0;
	AdcHandle_Right.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle_Right.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle_Right.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle_Right.Init.NbrOfConversion = 1;
	AdcHandle_Right.Init.DMAContinuousRequests = DISABLE;
	AdcHandle_Right.Init.EOCSelection = DISABLE;
	Which_ADC = "RIGHT";
	if (HAL_ADC_Init(&AdcHandle_Right) != HAL_OK) {
		/* Initialization Error */
		logme("fail at HAL_ADC_Init\r\n");
		Error_Handler_Right();
	}

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig_Right.Channel = ADCx_Right_CHANNEL;
	sConfig_Right.Rank = 2;
	sConfig_Right.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig_Right.Offset = 0;

	if (HAL_ADC_ConfigChannel(&AdcHandle_Right, &sConfig_Right) != HAL_OK) {
		/* Channel Configuration Error */
		logme("fail at HAL_ADC_ConfigChannel\r\n");
		Error_Handler_Right();
	}
}

static void Mux_Right_s0(char result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_RESET);
}

static void Mux_Right_s1(char result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_RESET);
}

static void Mux_Right_s2(char result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_RESET);
}

static void Mux_Right_s3(char result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_RESET);
}

static void Mux_Right_Open_by_Pin(int num) {
	char binary[4] = { 0, };
	for (short i = 0; i < 4; i++) {
		binary[i] = num % 2;
		num /= 2;
	}
	Mux_Right_s0(binary[0]);
	Mux_Right_s1(binary[1]);
	Mux_Right_s2(binary[2]);
	Mux_Right_s3(binary[3]);
	task_sleep(1);
}

static int Read_Right_ADC() {
	int uhADCxConvertedValue = 0;
	if (HAL_ADC_Start(&AdcHandle_Right) != HAL_OK) {
		/* Start Conversation Error */
		logme("fail at HAL_ADC_Start\r\n");
		Error_Handler_Right();
	}
	HAL_ADC_PollForConversion(&AdcHandle_Right, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&AdcHandle_Right) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG) {
		/*##-5- Get the converted value of regular channel #######################*/
		uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle_Right);
	}

	return uhADCxConvertedValue;
}
static void Read_Right_Mux_Data(short Column) {
	//Init all Right pin
	HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
	for (short Row = 0; Row < 48; Row++) {
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Right_Open_by_Pin(Row % 16);
			Foot_Array[47 - Column][Row + 48] = Read_Right_ADC();
			//printf("right %d box %d number\n",line,Read_Right_ADC());
		} else if (Row / 16 == 1) {
			//First Mux Operate
			if (Row == 16) {
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Right_Open_by_Pin(Row % 16);
			Foot_Array[47 - Column][Row + 48] = Read_Right_ADC();
			//printf("right %d box %d number\n",line,Read_Right_ADC());
		} else if (Row / 16 == 2) {
			//First Mux Operate
			if (Row == 32) {
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Right_Open_by_Pin(Row % 16);
			Foot_Array[47 - Column][Row + 48] = Read_Right_ADC();
			//printf("right %d box %d number\n",line,Read_Right_ADC());
		} else {
			printf("The mux number is too high\n");
		}
	}
}

#endif /* APP_FOOT_ADC_RIGHT_H_ */



