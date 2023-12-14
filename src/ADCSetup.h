#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

volatile bool adcDataAvailable __attribute__ ((aligned));
volatile uint16_t adcData[16];
constexpr uint32_t INTERVAL = 1000; // ms.
uint32_t t = millis();

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

// Handle DMA2 stream4 global interrupt.
extern "C" void DMA2_Stream0_IRQHandler(void) {
  HAL_DMA_IRQHandler(&hdma_adc1);
}

// ADC conversion complete callback.
extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) { 
  UNUSED(hadc); 
  adcDataAvailable = true;
}

void adcInit(void) {
  ADC_ChannelConfTypeDef sConfig = {0};

  __HAL_RCC_ADC1_CLK_ENABLE();
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 16;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
    while(1);
  
  hdma_adc1.Instance = DMA2_Stream0;
  hdma_adc1.Init.Channel = DMA_CHANNEL_0;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_NORMAL;
  hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
  hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    while(1);

  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  for (uint32_t i=1; i<=16; i++) {
    sConfig.Rank = i;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
      while(1);
  }
}

void dmaInit(void) {
  __HAL_RCC_DMA2_CLK_ENABLE();
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}