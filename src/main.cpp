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

int calVar = 0;
int test = 0;

float slopeAvg = 1; //average slope from slopeCalc



//One time executed code
void setup() 
{
  //cals func to setup MCU clock settings
  SystemClock_Config();
  //sets up I2C pins
  Wire.setSDA(PB7);
  Wire.setSCL(PB6); 
  Wire.begin();
  //set bit count of ADC
  analogReadResolution(16); 
  //button push interupt setup
  pinMode(PC13, INPUT_PULLUP); //enables PC13 pullup resistor
  pinMode(PC14, INPUT_PULLUP);
  pinMode(PC15, INPUT_PULLUP);
  //setup test LED
  pinMode(PA15, OUTPUT);
  //voltage opto gpio setup
  pinMode(PA0, OUTPUT); //200v range divider
  pinMode(PA1, OUTPUT); //200mv range
  pinMode(PB4, OUTPUT); //200mv range opamp output
  pinMode(PA2, OUTPUT); //10v range
  pinMode(PA3, OUTPUT); //2v range
  pinMode(PA4, OUTPUT); //master
  //amps Opto GPIO setup
  pinMode(PA5, OUTPUT); //master amps
  pinMode(PA11, OUTPUT); //10A AOpto1
  pinMode(PA10, OUTPUT); //1A AOpto2
  pinMode(PA9, OUTPUT); //100mA AOpto3
  pinMode(PA8, OUTPUT); //10mA AOpto4
  pinMode(PD8, OUTPUT); //1mA AOpto5

  //turns off ohms function, adding interferance to measurements
  //Ohms Opto GPIO setup
  pinMode(PA6, OUTPUT); //ohms
  pinMode(PA12, OUTPUT); //master Ohms control
  digitalWrite(PA6, HIGH);

  //Interupt HELL
  attachInterrupt(digitalPinToInterrupt(PC13), InteruptBoot1, FALLING); //triggers interupt function if PC13 goes low (triggers on falling edge)
  attachInterrupt(digitalPinToInterrupt(PC14), InteruptBoot2, FALLING);
  //start boot screen

  bootScreen();
  //slopeCalc();

 
 
  //Interupt HELL
  while(calVar < 1)
  {
    LedBlink();
    if(test == 1)
    {
      //LedBlink();
      slopeAvg = slopeCalc();
      calVar = 1;
    }
  }



  //calScreen(choice); /////need to continuously call the function to allow the variable being sent to change
  delay(1000);
  //measuremnet interupt setup (reuses the cal interupt gpio pins(buttons))
  attachInterrupt(digitalPinToInterrupt(PC13), InteruptVolts, FALLING); //triggers interuptVolts function if PC13 goes low (triggers on falling edge)
  attachInterrupt(digitalPinToInterrupt(PC14), InteruptAmps, FALLING); //LOW or RISING or CHANGE
  attachInterrupt(digitalPinToInterrupt(PC15), InteruptOhms, FALLING);
}



//Repeated code loop (allows the processor to continue opperating without needing manueal reset)
void loop() {
  //LedBlink();

  displayADC(configTemp, config2Temp, slopeAvg);
  LedBlink();
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
  //resets range after conpleating a full sweep
  //  - fix so full sweep not required (tOuCh sCrEeN yEt?????????)
  //itterate each time the button is pressed unless a diffrent fuction has been selected
  if(configTemp != 1)
  {
    configTemp = 1; //sets measure function 
    config2Temp = 1; //sets starting range for autoranging
  }
  else if(config2Temp != 4)
  {
    config2Temp = config2Temp + 1;
  }
  else
  {
    config2Temp = 1;
  }
  //testing on 2V range
  //config2Temp = 3; //sets starting range for autoranging
}



//amps button response
void InteruptAmps()
{
  //resets range after conpleating a full sweep
  //  - fix so full sweep not required (tOuCh sCrEeN yEt?????????)
  //itterate each time the button is pressed unless a diffrent fuction has been selected
  if(configTemp != 2)
  {
    configTemp = 2; //sets measure function 
    config2Temp = 1; //sets starting range for autoranging
  }
  else if(config2Temp != 4)
  {
    config2Temp = config2Temp + 1;
  }
  else
  {
    config2Temp = 1;
  }
}



//ohms button response
void InteruptOhms()
{

}







//Interupt HELL
void InteruptBoot1()
{
  test = 1;
}


void InteruptBoot2()
{
  calVar = 2;
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

