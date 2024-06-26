/*
 * FOOT_ADC.c
 *
 *  Created on: 2020. 9. 1.
 *      Author: Heo
 */

#include <ubinos.h>

#if (FOOT_ORIGINAL__USE_MULTICHANNEL != 1)

#include <FOOT_ADC.h>

/**************************************************/
/******************** ADC *************************/
/**************************************************/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
	GPIO_InitTypeDef GPIO_InitStruct;
	static DMA_HandleTypeDef hdma_adc;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC3 Periph clock enable */
	ADCx_Left_CLOCK_ENABLE();
	/* Enable GPIO clock ****************************************/
	ADCx_Left_CHANNEL_GPIO_CLOCK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* ADC3 Channel8 GPIO pin configuration */
	GPIO_InitStruct.Pin = ADCx_Left_CHANNEL_PIN|ADCx_Right_CHANNEL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ADCx_Left_CHANNEL_GPIO_PORT, &GPIO_InitStruct);


	//DMA
	DMAx_CLOCK_ENABLE();

	hdma_adc.Instance = ADCx_DMA_STREAM;

	hdma_adc.Init.Channel = ADCx_DMA_CHANNEL;
	hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc.Init.Mode = DMA_CIRCULAR;
	hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;
	if (HAL_DMA_Init(&hdma_adc) != HAL_OK)
	    {
	      Error_Handler();
	    }

	/* Associate the initialized DMA handle to the the ADC handle */
	__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

	/*##-4- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
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

	/*##-1- Reset peripherals ##################################################*/
	ADCx_Left_CLOCK_DISABLE();

	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* De-initialize the ADC3 Channel8 GPIO pin */
	HAL_GPIO_DeInit(ADCx_Left_CHANNEL_GPIO_PORT, ADCx_Left_CHANNEL_PIN|ADCx_Right_CHANNEL_PIN);

	/*##-3- Disable the DMA Streams ############################################*/
	/* De-Initialize the DMA Stream associate to transmission process */
	HAL_DMA_DeInit(hadc->DMA_Handle);

	/*##-4- Disable the NVIC for DMA ###########################################*/
	HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);
}

void ADCx_DMA_IRQHandler(void) {
	HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle) {
	/* Turn LED1 on: Transfer process is correct */

	Foot_Array[47 - adc_col][adc_row] = adcVal[0];
	Foot_Array[47 - adc_col][adc_row + 48] = adcVal[1];
}

/**************************************************/
/******************** left side ******************/
/**************************************************/
void Error_Handler(void) {
	/* Turn LED3 on */
	Led_Turn_on("LED");
}

void Mux_Init() {
	//LEFT MUX Init
	Mux_Left_S0_ENABLE();
	Mux_Left_S1_ENABLE();
	Mux_Left_S2_ENABLE();
	Mux_Left_S3_ENABLE();
	Mux_Left_Mux_1_EN_ENABLE();
	Mux_Left_Mux_2_EN_ENABLE();
	Mux_Left_Mux_3_EN_ENABLE();
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

	//left
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

	//right
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

	//ADC INIT Start
	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle.Instance = ADCx_Left_INSTANCE;

	AdcHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
	AdcHandle.Init.Resolution = ADC_RESOLUTION_8B;
	AdcHandle.Init.ScanConvMode = ENABLE;
	AdcHandle.Init.ContinuousConvMode = ENABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion = 0;
	AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion = 2;
	AdcHandle.Init.DMAContinuousRequests = ENABLE;
	AdcHandle.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	if (HAL_ADC_Init(&AdcHandle) != HAL_OK) {
		/* Initialization Error */
		logme("fail at HAL_ADC_Init\r\n");
		Error_Handler();
	}

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig.Channel = ADCx_Left_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;

	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
		/* Channel Configuration Error */
		logme("fail at HAL_ADC_ConfigChannel\r\n");
		Error_Handler();
	}

	/*##-3- Configure ADC regular channel ######################################*/
	sConfig.Channel = ADCx_Right_CHANNEL;
	sConfig.Rank = 2;
	if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
		/* Channel Configuration Error */
		logme("fail at HAL_ADC_ConfigChannel\r\n");
		Error_Handler();
	}


	//Init all Left pin
	HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);
	//Init all Left pin
	HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
	task_sleepms(1);
}

void Mux_s0(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S0_PORT, Mux_Left_S0_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S0_PORT, Mux_Right_S0_PIN, GPIO_PIN_RESET);
	}
}

void Mux_s1(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S1_PORT, Mux_Left_S1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S1_PORT, Mux_Right_S1_PIN, GPIO_PIN_RESET);
	}
}

void Mux_s2(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S2_PORT, Mux_Left_S2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S2_PORT, Mux_Right_S2_PIN, GPIO_PIN_RESET);
	}
}

void Mux_s3(short result) {
	if (result == 1) {
		HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_SET);
	} else if (result == 0) {
		HAL_GPIO_WritePin(Mux_Left_S3_PORT, Mux_Left_S3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Mux_Right_S3_PORT, Mux_Right_S3_PIN, GPIO_PIN_RESET);
	}
}

void Mux_Open_by_Pin(int num) {
	short binary[4] = { 0, };
	for (short i = 0; i < 4; i++) {
		binary[i] = num % 2;
		num /= 2;
	}
	Mux_s0(binary[0]);
	Mux_s1(binary[1]);
	Mux_s2(binary[2]);
	Mux_s3(binary[3]);
	task_sleep(1);
}

void Read_Mux_Data(short Column) {
	for (short Row = 0; Row < 48; Row++) {
		printf("%d\n", Row);
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);
			adc_col=Column,adc_row=Row;
		} else if (Row / 16 == 1) {
			//First Mux Operate
			if (Row == 16) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);
			adc_col=Column,adc_row=Row;
		} else if (Row / 16 == 2) {
			//First Mux Operate
			if (Row == 32) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_RESET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_RESET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);
			adc_col=Column,adc_row=Row;
		} else {
			printf("The mux number is too high\n");
		}
	}
}

/////////test code
void Read_Mux_Data_task1(void *data) {
	short Column = *(int*) data;
	for (short Row = 0; Row < 16; Row++) {
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);

			//left
			if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
				/* Start Conversation Error */
				logme("fail at HAL_ADC_Start\r\n");
				Error_Handler();
			}
			HAL_ADC_PollForConversion(&AdcHandle, 10);
			Foot_Array[47 - Column][Row] = HAL_ADC_GetValue(&AdcHandle);

			//right
			if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
				/* Start Conversation Error */
				logme("fail at HAL_ADC_Start\r\n");
				Error_Handler();
			}
			HAL_ADC_PollForConversion(&AdcHandle, 10);

			Foot_Array[47 - Column][Row + 48] = HAL_ADC_GetValue(&AdcHandle);

		} else {
			printf("The mux number is too high\n");
		}
	}
}
void Read_Mux_Data_task2(void *data) {
	short Column = *(int*) data;
	for (short Row = 16; Row < 32; Row++) {
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);

			//left
			if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
				/* Start Conversation Error */
				logme("fail at HAL_ADC_Start\r\n");
				Error_Handler();
			}
			HAL_ADC_PollForConversion(&AdcHandle, 10);
			Foot_Array[47 - Column][Row] = HAL_ADC_GetValue(&AdcHandle);

			//right
			if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
				/* Start Conversation Error */
				logme("fail at HAL_ADC_Start\r\n");
				Error_Handler();
			}
			HAL_ADC_PollForConversion(&AdcHandle, 10);

			Foot_Array[47 - Column][Row + 48] = HAL_ADC_GetValue(&AdcHandle);

		} else {
			printf("The mux number is too high\n");
		}
	}
}
void Read_Mux_Data_task3(void *data) {
	short Column = *(int*) data;
	for (short Row = 32; Row < 48; Row++) {
		if (Row / 16 == 0) {
			//First Mux Operate
			if (Row == 0) {
				//left
				HAL_GPIO_WritePin(Mux_Left_Mux_1_EN_PORT, Mux_Left_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Left_Mux_2_EN_PORT, Mux_Left_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Left_Mux_3_EN_PORT, Mux_Left_Mux_3_EN_PIN, GPIO_PIN_SET);
				//right
				HAL_GPIO_WritePin(Mux_Right_Mux_1_EN_PORT, Mux_Right_Mux_1_EN_PIN, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(Mux_Right_Mux_2_EN_PORT, Mux_Right_Mux_2_EN_PIN, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Mux_Right_Mux_3_EN_PORT, Mux_Right_Mux_3_EN_PIN, GPIO_PIN_SET);
				task_sleep(1);
			}
			/* 4bit mux is operated by S0~ S3 pin.
			 * If all pin is low, Y0 pin is open
			 * If all pin is high, Y15 pin is open*/
			Mux_Open_by_Pin(Row % 16);

			//left
			if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
				/* Start Conversation Error */
				logme("fail at HAL_ADC_Start\r\n");
				Error_Handler();
			}
			HAL_ADC_PollForConversion(&AdcHandle, 10);
			Foot_Array[47 - Column][Row] = HAL_ADC_GetValue(&AdcHandle);

			//right
			if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
				/* Start Conversation Error */
				logme("fail at HAL_ADC_Start\r\n");
				Error_Handler();
			}
			HAL_ADC_PollForConversion(&AdcHandle, 10);

			Foot_Array[47 - Column][Row + 48] = HAL_ADC_GetValue(&AdcHandle);

		} else {
			printf("The mux number is too high\n");
		}
	}
}

int Read_Left_ADC() {
	int uhADCxConvertedValue = 0;
	if (HAL_ADC_Start(&AdcHandle) != HAL_OK) {
		/* Start Conversation Error */
		logme("fail at HAL_ADC_Start\r\n");
		Error_Handler();
	}
	HAL_ADC_PollForConversion(&AdcHandle, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG) {
		/*##-5- Get the converted value of regular channel #######################*/
		uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
	}
	Foot_Array[47][0] = uhADCxConvertedValue;
	HAL_ADC_PollForConversion(&AdcHandle, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG) {
		/*##-5- Get the converted value of regular channel #######################*/
		uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
	}
	Foot_Array[47][48] = uhADCxConvertedValue;
	return 1;
}

#endif /* (FOOT_ORIGINAL__USE_MULTICHANNEL != 1) */
