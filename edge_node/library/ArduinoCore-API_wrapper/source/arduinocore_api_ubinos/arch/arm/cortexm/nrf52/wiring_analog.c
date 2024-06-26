/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ubinos.h>

#if (INCLUDE__ARDUINOCORE_API == 1)
#if (UBINOS__BSP__NRF52_NRF52XXX == 1)

#include <Arduino.h>

#include <ubinos/bsp/arch.h>

#include "_variant.h"

static int _readResolution = 10;
static uint8_t _is_nrf_drv_saadc_init = 0;

static uint32_t _saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
static uint32_t _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_6;

void analogReadResolution(int res)
{
  _readResolution = res;
}

/*
 * Internal Reference is +/-0.6V, with an adjustable gain of 1/6, 1/5, 1/4,
 * 1/3, 1/2 or 1, meaning 3.6, 3.0, 2.4, 1.8, 1.2 or 0.6V for the ADC levels.
 *
 * External Reference is VDD/4, with an adjustable gain of 1, 2 or 4, meaning
 * VDD/4, VDD/2 or VDD for the ADC levels.
 *
 * Default settings are internal reference with 1/6 gain (GND..3.6V ADC range)
 */
void analogReference(uint8_t mode)
{
  switch ( mode ) {
    case AR_VDD4:
      _saadcReference = SAADC_CH_CONFIG_REFSEL_VDD1_4;
      _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_4;
      break;
    case AR_INTERNAL_3_0:
      _saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
      _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_5;
      break;
    case AR_INTERNAL_2_4:
      _saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
      _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_4;
      break;
    case AR_INTERNAL_1_8:
      _saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
      _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_3;
      break;
    case AR_INTERNAL_1_2:
      _saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
      _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_2;
      break;
    case AR_DEFAULT:
    case AR_INTERNAL:
    default:
      _saadcReference = SAADC_CH_CONFIG_REFSEL_Internal;
      _saadcGain      = SAADC_CH_CONFIG_GAIN_Gain1_6;
      break;

  }
}


int analogRead(pin_size_t pinNumber)
{
    ret_code_t err_code;
    arduino_a_pin_t const * a_pin;
    nrf_saadc_value_t value;

    do
    {
        if (pinNumber >= NUM_ANALOG_INPUTS)
        {
            logme("pinNumber is out of range");
            break;
        }

        a_pin = &_g_a_pin_map[pinNumber];

        if (a_pin->enable == 0)
        {
            logme("Not enabled");
            break;
        }

        if (!_is_nrf_drv_saadc_init)
        {
            err_code = nrf_drv_saadc_init(NULL, NULL);
            APP_ERROR_CHECK(err_code);

            if (_readResolution <= 8) {
                nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_8BIT);
            } else if (_readResolution <= 10) {
                nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_10BIT);
            } else if (_readResolution <= 12) {
                nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_12BIT);
            } else {
                nrf_saadc_resolution_set(NRF_SAADC_RESOLUTION_14BIT);
            }

            _is_nrf_drv_saadc_init = 1;
        }

        nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(a_pin->adc_input);
        channel_config.reference = _saadcReference;
        channel_config.gain = _saadcGain;

        err_code = nrf_drv_saadc_channel_init(a_pin->channel, &channel_config);
        APP_ERROR_CHECK(err_code);

        err_code = nrf_drv_saadc_sample_convert(a_pin->channel, &value);
        APP_ERROR_CHECK(err_code);

        break;
    } while(1);

    return (int) value;
}

#endif /* (UBINOS__BSP__NRF52_NRF52XXX == 1) */
#endif /* (INCLUDE__ARDUINOCORE_API == 1) */

