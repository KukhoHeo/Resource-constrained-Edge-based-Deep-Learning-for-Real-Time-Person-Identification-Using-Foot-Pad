/*
 * FOOT_ADC.h
 *
 *  Created on: 2020. 6. 9.
 *      Author: Heo
 */

#ifndef APP_FOOT_ADC_LEFT_H_
#define APP_FOOT_ADC_LEFT_H_
#include <FOOT_HARD.h>
//GPIO Init Structure
static GPIO_InitTypeDef GPIO_InitStruct;

/* ADC handler declaration */
ADC_HandleTypeDef AdcHandle_Left;

static void Error_Handler_Left(void) {
	/* Turn LED3 on */
	Led_Turn_on("RED");
}
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
	if (!strcmp(Which_ADC, "LEFT")) {
		GPIO_InitTypeDef GPIO_InitStruct;

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* ADC3 Periph clock enable */
		ADCx_Left_CLOCK_ENABLE();
		/* Enable GPIO clock ****************************************/
		ADCx_Left_CHANNEL_GPIO_CLOCK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* ADC3 Channel8 GPIO pin configuration */
		GPIO_InitStruct.Pin = ADCx_Left_CHANNEL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADCx_Left_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
	} else if (!strcmp(Which_ADC, "RIGHT")) {
		GPIO_InitTypeDef GPIO_InitStruct;

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* ADC3 Periph clock enable */
		ADCx_Right_CLOCK_ENABLE();
		/* Enable GPIO clock ****************************************/
		ADCx_Right_CHANNEL_GPIO_CLOCK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* ADC3 Channel8 GPIO pin configuration */
		GPIO_InitStruct.Pin = ADCx_Right_CHANNEL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADCx_Right_CHANNEL_GPIO_PORT, &GPIO_InitStruct);
	} else {
		printf("Wrong ADC INIT\n");
	}
}

/**
 * @brief ADC MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO to their default state
 * @param hadc: ADC handle pointer
 * @retval None
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc) {
	if (!strcmp(Which_ADC, "LEFT")) {
		/*##-1- Reset peripherals ##################################################*/
		ADCx_Left_FORCE_RESET();
		ADCx_Left_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* De-initialize the ADC3 Channel8 GPIO pin */
		HAL_GPIO_DeInit(ADCx_Left_CHANNEL_GPIO_PORT, ADCx_Left_CHANNEL_PIN);
	} else if (!strcmp(Which_ADC, "LEFT")) {
		/*##-1- Reset peripherals ##################################################*/
		ADCx_Right_FORCE_RESET();
		ADCx_Right_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* De-initialize the ADC3 Channel8 GPIO pin */
		HAL_GPIO_DeInit(ADCx_Right_CHANNEL_GPIO_PORT, ADCx_Right_CHANNEL_PIN);
	} else {
		printf("Wrong ADC DEINIT\n");
	}
}

static void Mux_Left_Init() {
	//LEFT MUX Init
	Mux_Left_S0_ENABLE();
	Mux_Left_S1_ENABLE();
	Mux_Left_S2_ENABLE();
	Mux_Left_S3_ENABLE();
	Mux_Left_Mux_1_EN_ENABLE();
	Mux_Left_Mux_2_EN_ENABLE();
	Mux_Left_Mux_3_EN_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = Mux_Left_S0_PIN;
	HAL_GPIO_Init(Mux_Left_S0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_S1_PIN;
	HAL_GPIO_Init(Mux_Left_S1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_S2_PIN;
	HAL_GPIO_Init(Mux_Left_S2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_S3_PIN;
	HAL_GPIO_Init(Mux_Left_S3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_Mux_1_EN_PIN;
	HAL_GPIO_Init(Mux_Left_Mux_1_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_Mux_2_EN_PIN;
	HAL_GPIO_Init(Mux_Left_Mux_2_EN_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = Mux_Left_Mux_3_EN_PIN;
	HAL_GPIO_Init(Mux_Left_Mux_3_EN_PORT, &GPIO_InitStruct);

	//LEFT ADC INIT Start
	ADC_ChannelConfTypeDef sConfig_Left;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle_Left.Instance = ADCx_Left_INSTANCE;

	AdcHandle_Left.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle_Left.Init.Resolution = ADC_RESOLUTION_12B;
	AdcHandle_Left.Init.ScanConvMode = DISABLE;
	AdcHandle_Left.Init.ContinuousConvMode = DISABLE;
	AdcHandle_Left.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle_Left.Init.NbrOfDiscConversion = 0;
	AdcHandle_Left.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle_Left.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle_Left.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle_Left.Init.NbrOfConversion = 1;
	AdcHandle_Left.Init.DMAContinuousRequests = DISABLE;
	AdcHandle_Left.Init.EOCSelection = DISABLE;
	Which_ADC = "LEFT";
	if (HAL_ADC_Init(&AdcHandle_Left) != HAL_OK) {
		/* Initialization Error */
		logme("fail at HAL_ADC_Init\r\n");
		Error_Handler_Left();
	}

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig_Left.Channel = ADCx_Left_CHANNEL;
	sConfig_Left.Rank = 1;
	sConfig_Left.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig_Left.Offset = 0;

	if (HAL_ADC_ConfigChannel(&AdcHandle_Left, &sConfig_Left) != HAL_OK) {
		/* Channel Configuration Error */
		logme("fail at HAL_ADC_ConfigChannel\r\n");
		Error_Handler_Left();
	}
}

static void Mux_Left_s0(short result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_RESET);
}

static void Mux_Left_s1(short result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_RESET);
}

static void Mux_Left_s2(short result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_RESET);
}

static void Mux_Left_s3(short result) {
	if (result == 1)
		HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_SET);
	if (result == 0)
		HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_RESET);
}

static void Mux_Left_Open_by_Pin(int num) {
	short binary[4] = { 0, };
	for (short i = 0; i < 4; i++) {
		binary[i] = num % 2;
		num /= 2;
	}
	Mux_Left_s0(binary[0]);
	Mux_Left_s1(binary[1]);
	Mux_Left_s2(binary[2]);
	Mux_Left_s3(binary[3]);
	task_sleep(1);
}

static int Read_Left_ADC() {
	int uhADCxConvertedValue = 0;
	if (HAL_ADC_Start(&AdcHandle_Left) != HAL_OK) {
		/* Start Conversation Error */
		logme("fail at HAL_ADC_Start\r\n");
		Error_Handler_Left();
	}
	HAL_ADC_PollForConversion(&AdcHandle_Left, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&AdcHandle_Left) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG) {
		/*##-5- Get the converted value of regular channel #######################*/
		uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle_Left);
	}
	return uhADCxConvertedValue;
}
static void Read_Left_Mux_Data(short Column) {
	//Init all Left pin
	HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);
	for (short Row = 0; Row < 48; Row++) {
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Left_Open_by_Pin(Row % 16);
			Foot_Array[47 - Column][Row] = Read_Left_ADC();
			//printf("left %d box %d number\n", line, Read_Left_ADC());
		} else if (Row / 16 == 1) {
			//First Mux Operate
			if (Row == 16) {
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Left_Open_by_Pin(Row % 16);
			Foot_Array[47 - Column][Row] = Read_Left_ADC();
			//printf("left %d box %d number\n", line, Read_Left_ADC());
		} else if (Row / 16 == 2) {
			//First Mux Operate
			if (Row == 32) {
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Left_Open_by_Pin(Row % 16);
			Foot_Array[47 - Column][Row] = Read_Left_ADC();
			//printf("left %d box %d number\n", line, Read_Left_ADC());
		} else {
			printf("The mux number is too high\n");
		}
	}
}

#endif /* APP_FOOT_ADC_LEFT_H_ */
