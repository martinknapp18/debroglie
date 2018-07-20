#include "timer_oc_api.h"

#include "PeripheralPins.h"
#include "cmsis.h"
#include "mbed_error.h"
#include "pinmap.h"
#include "pwmout_device.h"

static TIM_HandleTypeDef TimHandle;

void timer_oc_init(timer_oc_t *obj, PinName pin) {
  // Get the peripheral name from the pin and assign it to the object
  // Reusing the pinmapings for PWM since they are the same as timer outputs.
  obj->pwm = (PWMName)pinmap_peripheral(pin, PinMap_PWM);
  MBED_ASSERT(obj->pwm != (PWMName)NC);

  // Get the functions (timer channel, (non)inverted) from the pin and assign it
  // to the object
  uint32_t function = pinmap_function(pin, PinMap_PWM);
  MBED_ASSERT(function != (uint32_t)NC);
  obj->channel = STM_PIN_CHANNEL(function);
  obj->inverted = STM_PIN_INVERTED(function);

// Enable TIM clock
#if defined(TIM1_BASE)
  if (obj->pwm == PWM_1) {
    __HAL_RCC_TIM1_CLK_ENABLE();
  }
#endif
#if defined(TIM2_BASE)
  if (obj->pwm == PWM_2) {
    __HAL_RCC_TIM2_CLK_ENABLE();
  }
#endif
#if defined(TIM3_BASE)
  if (obj->pwm == PWM_3) {
    __HAL_RCC_TIM3_CLK_ENABLE();
  }
#endif
#if defined(TIM4_BASE)
  if (obj->pwm == PWM_4) {
    __HAL_RCC_TIM4_CLK_ENABLE();
  }
#endif
#if defined(TIM5_BASE)
  if (obj->pwm == PWM_5) {
    __HAL_RCC_TIM5_CLK_ENABLE();
  }
#endif
#if defined(TIM8_BASE)
  if (obj->pwm == PWM_8) {
    __HAL_RCC_TIM8_CLK_ENABLE();
  }
#endif
#if defined(TIM9_BASE)
  if (obj->pwm == PWM_9) {
    __HAL_RCC_TIM9_CLK_ENABLE();
  }
#endif
#if defined(TIM10_BASE)
  if (obj->pwm == PWM_10) {
    __HAL_RCC_TIM10_CLK_ENABLE();
  }
#endif
#if defined(TIM11_BASE)
  if (obj->pwm == PWM_11) {
    __HAL_RCC_TIM11_CLK_ENABLE();
  }
#endif
#if defined(TIM12_BASE)
  if (obj->pwm == PWM_12) {
    __HAL_RCC_TIM12_CLK_ENABLE();
  }
#endif
#if defined(TIM13_BASE)
  if (obj->pwm == PWM_13) {
    __HAL_RCC_TIM13_CLK_ENABLE();
  }
#endif
#if defined(TIM14_BASE)
  if (obj->pwm == PWM_14) {
    __HAL_RCC_TIM14_CLK_ENABLE();
  }
#endif
#if defined(TIM15_BASE)
  if (obj->pwm == PWM_15) {
    __HAL_RCC_TIM15_CLK_ENABLE();
  }
#endif
#if defined(TIM16_BASE)
  if (obj->pwm == PWM_16) {
    __HAL_RCC_TIM16_CLK_ENABLE();
  }
#endif
#if defined(TIM17_BASE)
  if (obj->pwm == PWM_17) {
    __HAL_RCC_TIM17_CLK_ENABLE();
  }
#endif
#if defined(TIM18_BASE)
  if (obj->pwm == PWM_18) {
    __HAL_RCC_TIM18_CLK_ENABLE();
  }
#endif
#if defined(TIM19_BASE)
  if (obj->pwm == PWM_19) {
    __HAL_RCC_TIM19_CLK_ENABLE();
  }
#endif
#if defined(TIM20_BASE)
  if (obj->pwm == PWM_20) {
    __HAL_RCC_TIM20_CLK_ENABLE();
  }
#endif
#if defined(TIM21_BASE)
  if (obj->pwm == PWM_21) {
    __HAL_RCC_TIM21_CLK_ENABLE();
  }
#endif
#if defined(TIM22_BASE)
  if (obj->pwm == PWM_22) {
    __HAL_RCC_TIM22_CLK_ENABLE();
  }
#endif
  // Configure GPIO
  pinmap_pinout(pin, PinMap_PWM);

  obj->pin = pin;
  obj->period = 0;
  obj->pulse = 0;
  obj->prescaler = 1;
}

void timer_oc_free(timer_oc_t *obj) {
  // Configure GPIO
  pin_function(obj->pin, STM_PIN_DATA(STM_MODE_INPUT, GPIO_NOPULL, 0));
}

void timer_oc_start(timer_oc_t *obj, int us_period, int num_repetitions,
                    uint32_t *ticks) {
  TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  uint32_t PclkFreq = 0;
  uint32_t APBxCLKDivider = RCC_HCLK_DIV1;
  uint8_t i = 0;

  __HAL_TIM_DISABLE(&TimHandle);

  // Get clock configuration
  // Note: PclkFreq contains here the Latency (not used after)
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &PclkFreq);

  /*  Parse the pwm / apb mapping table to find the right entry */
  while (pwm_apb_map_table[i].pwm != obj->pwm) {
    i++;
  }

  if (pwm_apb_map_table[i].pwm == 0) {
    error("Unknown OC instance");
  }

  if (pwm_apb_map_table[i].pwmoutApb == PWMOUT_ON_APB1) {
    PclkFreq = HAL_RCC_GetPCLK1Freq();
    APBxCLKDivider = RCC_ClkInitStruct.APB1CLKDivider;
  } else {
#if !defined(PWMOUT_APB2_NOT_SUPPORTED)
    PclkFreq = HAL_RCC_GetPCLK2Freq();
    APBxCLKDivider = RCC_ClkInitStruct.APB2CLKDivider;
#endif
  }

  /* By default use, 1us as SW pre-scaler */
  obj->prescaler = 1;
  // TIMxCLK = PCLKx when the APB prescaler = 1 else TIMxCLK = 2 * PCLKx
  if (APBxCLKDivider == RCC_HCLK_DIV1) {
    TimHandle.Init.Prescaler = (((PclkFreq) / 1000000)) - 1; // 1 us tick
  } else {
    TimHandle.Init.Prescaler = (((PclkFreq * 2) / 1000000)) - 1; // 1 us tick
  }
  TimHandle.Init.Period = (us_period - 1);

  /*  In case period or pre-scalers are out of range, loop-in to get valid
   * values */
  while ((TimHandle.Init.Period > 0xFFFF) ||
         (TimHandle.Init.Prescaler > 0xFFFF)) {
    obj->prescaler = obj->prescaler * 2;
    if (APBxCLKDivider == RCC_HCLK_DIV1) {
      TimHandle.Init.Prescaler = (((PclkFreq) / 1000000) * obj->prescaler) - 1;
    } else {
      TimHandle.Init.Prescaler =
          (((PclkFreq * 2) / 1000000) * obj->prescaler) - 1;
    }
    TimHandle.Init.Period = (us_period - 1) / obj->prescaler;
    /*  Period decreases and prescaler increases over loops, so check for
     *  possible out of range cases */
    if ((TimHandle.Init.Period < 0xFFFF) &&
        (TimHandle.Init.Prescaler > 0xFFFF)) {
      error("Cannot initialize OC\n");
      break;
    }
  }

  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

  // Repetitions
  TimHandle.Init.RepetitionCounter = num_repetitions;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  for (int i = 0; i < num_repetitions; i++) {
    ticks[i] = ticks[i] / obj->prescaler;
  }

  if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK) {
    error("Cannot initialize OC\n");
  }

  // Save for future use
  obj->period = us_period;

  TIM_OC_InitTypeDef sConfig;

  TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);
  // Configure channels
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.Pulse = ticks[0];
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;
#if defined(TIM_OCIDLESTATE_RESET)
  sConfig.OCIdleState = TIM_OCIDLESTATE_SET;
#endif
#if defined(TIM_OCNIDLESTATE_RESET)
  sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_SET;
#endif

  int channel = 0;
  switch (obj->channel) {
  case 1:
    channel = TIM_CHANNEL_1;
    break;
  case 2:
    channel = TIM_CHANNEL_2;
    break;
  case 3:
    channel = TIM_CHANNEL_3;
    break;
  case 4:
    channel = TIM_CHANNEL_4;
    break;
  default:
    return;
  }

  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
    error("Cannot initialize OC\n");
  }

#if !defined(PWMOUT_INVERTED_NOT_SUPPORTED)
  if (obj->inverted) {
    HAL_TIMEx_OCN_Start(&TimHandle, channel);
  } else
#endif
  {
    if (HAL_TIM_PWM_Start_DMA(&TimHandle, channel, &ticks[1], num_repetitions) != HAL_OK) {
      error("Cannot start DMA\n");
    }
  }
  wait_ms(210);
  timer_oc_stop(obj);
}

void timer_oc_stop(timer_oc_t *obj) {
  printf("stopping 1\n");
  TimHandle.Instance = (TIM_TypeDef *)(obj->pwm);
  int channel = 0;
  switch (obj->channel) {
  case 1:
    channel = TIM_CHANNEL_1;
    break;
  case 2:
    channel = TIM_CHANNEL_2;
    break;
  case 3:
    channel = TIM_CHANNEL_3;
    break;
  case 4:
    channel = TIM_CHANNEL_4;
    break;
  default:
    return;
  }

  if (HAL_TIM_PWM_Stop_DMA(&TimHandle, channel) != HAL_OK) {
    error("Cannot start DMA\n");
  }
}
