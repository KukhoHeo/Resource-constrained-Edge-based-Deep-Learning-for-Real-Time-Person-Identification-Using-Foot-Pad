/*
 * Copyright (c) 2021 Sung Ho Park and CSOS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VARIANT_H_
#define VARIANT_H_

#define ADC_RESOLUTION              10

#define PWM_FREQUENCY               1000
#define PWM_MAX_DUTY_CYCLE          255

_Static_assert((ADC_RESOLUTION > 0) &&(ADC_RESOLUTION <= 32), "ADC_RESOLUTION must be 0 < x <= 32!");

enum // analog_reference
{
  AR_DEFAULT,
  AR_INTERNAL,          // 0.6V Ref * 6 = 0..3.6V
  AR_INTERNAL_3_0,      // 0.6V Ref * 5 = 0..3.0V
  AR_INTERNAL_2_4,      // 0.6V Ref * 4 = 0..2.4V
  AR_INTERNAL_1_8,      // 0.6V Ref * 3 = 0..1.8V
  AR_INTERNAL_1_2,      // 0.6V Ref * 2 = 0..1.6V
  AR_VDD4               // VDD/4 REF * 4 = 0..VDD
};

#endif /* VARIANT_H_ */

