#include "TimerOc.h"
#include "mbed.h"
#include "minig.h"

DigitalOut red{LED1};
DigitalOut blue{LED2};
DigitalOut green{LED3};
Ticker led_ticker;

void toggle_led() {
  static int i = 0;

  red = i & 1;
  blue = i & 2;
  green = i & 4;
  i++;
}
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of TIM instance */
#define TIMx TIM3

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE __HAL_RCC_TIM3_CLK_ENABLE
#define DMAx_CLK_ENABLE __HAL_RCC_DMA1_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL3_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE

#define TIMx_GPIO_CHANNEL3_PORT GPIOB
#define GPIO_PIN_CHANNEL3 GPIO_PIN_0

#define GPIO_AF_TIMx GPIO_AF2_TIM3

/* Definition for TIMx's DMA */
#define TIMx_CC3_DMA_INST DMA1_Stream7

/* Definition for ADCx's NVIC */
#define TIMx_DMA_IRQn DMA1_Stream7_IRQn
#define TIMx_DMA_IRQHandler DMA1_Stream7_IRQHandler

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
  static DMA_HandleTypeDef hdma_tim;

  /* Enable DMA clock */
  DMAx_CLK_ENABLE();

  /* Set the parameters to be configured */
  hdma_tim.Init.Channel = DMA_CHANNEL_5;
  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tim.Init.Mode = DMA_CIRCULAR;
  hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_tim.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_tim.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_tim.Init.PeriphBurst = DMA_PBURST_SINGLE;

  /* Set hdma_tim instance */
  hdma_tim.Instance = TIMx_CC3_DMA_INST;

  /* Link hdma_tim to hdma[TIM_DMA_ID_CC3] (channel3) */
  __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC3], hdma_tim);

  /* Initialize TIMx DMA handle */
  HAL_DMA_Init(htim->hdma[TIM_DMA_ID_CC3]);

  /*##-2- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(TIMx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIMx_DMA_IRQn);
}

uint32_t ticks[3] = {4, 8, 4};

// MiniG minig;
int main() {
  led_ticker.attach(toggle_led, .2);
  printf("\nhello debugging!\n");

  // minig.init();

  TimerOc timer_oc{PB_0_ALT0};
  printf("before start\n");
  timer_oc.start(100000 /* us*/, 3, ticks);
  printf("before wait\n");
  wait_ms(210);
  printf("after wait\n");
  timer_oc.stop();

  while (1) {
    // minig.run();
  }
}
