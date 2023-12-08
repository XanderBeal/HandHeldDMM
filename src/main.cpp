#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//I2C setup
#include <Wire.h>

//import files
#include "measure.hpp"
#include "cal.hpp"
#include "screenDisplay.hpp"

//function declarations
void LedBlink(void);
void InteruptVolts(void);
void InteruptAmps(void);
void InteruptOhms(void);

//variable declarations
double digitalValue = 0.00;// variable to store the value coming from the sensor
int configTemp = 0; //tells displayADC which mode is used
int config2Temp = 0; //tells displayADC which range is used
double offset = 0; //offset value for measurement cal
int display = 0; //triggers adc display and home screen clear






//One time executed code
void setup() {
  
  //cals func to setup MCU clock settings
  SystemClock_Config();
  
  //sets up I2C pins
  Wire.setSDA(PB7);
  Wire.setSCL(PB6); 
  Wire.begin();

  //from display.cpp
  analogReadResolution(16); //set bit count of ADC
  

  bootScreen();
  //press volts for cal
  //press nothing and wait for a delay before moving on, may need move on button


  //button push interupt setup
  pinMode(PC13, INPUT_PULLUP); //enables PC13 pullup resistor
  pinMode(PC14, INPUT_PULLUP);
  pinMode(PC15, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PC13), InteruptVolts, FALLING); //triggers interuptVolts function if PC13 goes low (triggers on falling edge)
  attachInterrupt(digitalPinToInterrupt(PC14), InteruptAmps, FALLING);
  attachInterrupt(digitalPinToInterrupt(PC15), InteruptOhms, FALLING);

  //setup test LED
  pinMode(PA15, OUTPUT);

  //voltage opto gpio's
  pinMode(PA0, OUTPUT); //200v range divider
  pinMode(PA1, OUTPUT); //200v range
  pinMode(PA2, OUTPUT); //10v range
  pinMode(PA3, OUTPUT); //2v range
  pinMode(PA4, OUTPUT); //master

  pinMode(PA6, OUTPUT); //ohms
  digitalWrite(PA6, HIGH);
  
}









//Repeated code loop (allows the processor to continue opperating without needing manueal reset)
void loop() {

  LedBlink();

  offset = cal();

  if(display == 1) //screen selection
  {
    displayADC(configTemp, config2Temp, offset);
  }
  

  //for faster run time and refresh, setup range and only change and run code for it if range changes
  // i.e. seperate range and measurement function calling

}












//test function 
void LedBlink()
{
  digitalWrite(PA15, HIGH);

  delay(30);

  digitalWrite(PA15, LOW);

  delay(30);
}


//volts button response
void InteruptVolts()
{

  //itterate each time the button is pressed unless a diffrent fuction has been selected
  if(configTemp != 1)
  {
    configTemp = 1; //sets measure function 
    config2Temp = 1; //sets starting range for autoranging
    display = 1; //sets screen to DisplayADC
  }
  else
  {
    config2Temp = config2Temp + 1;
  }

  //resets range after conpleating a full sweep
  //  - fix so full sweep not required (tOuCh sCrEeN yEt?????????)
  if( config2Temp == 4)
  {
    config2Temp = 1;
  }

  //testing on 2V range
  config2Temp = 3; //sets starting range for autoranging
}

//amps button response
void InteruptAmps()
{
  //itterate each time the button is pressed unless a diffrent fuction has been selected
  if(configTemp != 2)
  {
    configTemp = 2; //sets measure function 
    config2Temp = 1; //sets starting range for autoranging
    display = 1; //sets screen to DisplayADC
  }
  else
  {
    config2Temp = config2Temp + 1;
  }

  //resets range after conpleating a full sweep
  //  - fix so full sweep not required (tOuCh sCrEeN yEt?????????)
  if( config2Temp == 5)
  {
    config2Temp = 1;
  }
}

//ohms button response
void InteruptOhms()
{
  //itterate each time the button is pressed unless a diffrent fuction has been selected
  if(configTemp != 3)
  {
    configTemp = 3; //sets measure function 
    config2Temp = 1; //sets starting range for autoranging
    display = 1; //sets screen to DisplayADC
  }
  else
  {
    config2Temp = config2Temp + 1;
  }

  //resets range after conpleating a full sweep
  //  - fix so full sweep not required (tOuCh sCrEeN yEt?????????)
  if( config2Temp == 5)
  {
    config2Temp = 1;
  }
}






extern "C" void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}