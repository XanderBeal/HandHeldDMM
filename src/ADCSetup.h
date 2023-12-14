// #include <Arduino.h>

// #include "stm32f3xx_hal_sdadc.h"

// #include "stm32f3xx_hal.h"
// #define STM32F302xE

// ADC_HandleTypeDef hadc1;

// void setup() {
//   Serial.begin(115200);

//   // ADC1 init
//   hadc1.Instance = ADC1;
//   hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
//   hadc1.Init.Resolution = ADC_RESOLUTION_16B;
//   hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//   hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
//   hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//   hadc1.Init.ContinuousConvMode = DISABLE;
//   hadc1.Init.NbrOfConversion = 1;
//   hadc1.Init.DiscontinuousConvMode = DISABLE;
//   hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//   hadc1.Init.DMAContinuousRequests = DISABLE;
//   hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//   if (HAL_ADC_Init(&hadc1) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   // ADC1 regular channel0 configuration
//   ADC_ChannelConfTypeDef sConfig = {0};
//   sConfig.Channel = ADC_CHANNEL_1;
//   sConfig.Rank = ADC_REGULAR_RANK_1;
//   sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }

// void loop() {
//   HAL_ADC_Start(&hadc1);
//   if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) {
//     uint32_t value = HAL_ADC_GetValue(&hadc1);
//     Serial.println(value);
//   }
//   HAL_ADC_Stop(&hadc1);
//   delay(1000);
// }

// void Error_Handler(void)
// {
//   while(1)
//   {
//     Serial.println("Error occurred in ADC initialization or configuration.");
//     delay(1000);
//   }
// }