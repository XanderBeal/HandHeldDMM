#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "measure.hpp"
//I2C setup
#include <Wire.h>
//display setup
#include "screenDisplay.hpp"
extern Adafruit_SSD1306 display; //becasue "display" is defined in screenDisplay but also needs to be defined here, we can use and extern to define it I guess?
//file variable declarations
int config = 0; //tells displayADC which mode is used
int config2 = 0; //tells displayADC which range is used
double rangeMult = 1; //multiplier to acount for range on display



//main function, dictates all ADC function related control
//  - take ADC measurement, display measure screen
void displayADC(int config, int config2, double slopeAvg)
{ 
  double ampsMult = 0;
  double Vin = 0;
  //volts/amps/ohms setup
  switch(config) 
  {
    case 1: //volts 
      digitalWrite(PA5, LOW); //ADCOpto1 (Master current range)
      digitalWrite(PA4, HIGH); //ADCOpto2 (input to buffer)
      

      voltsRange(config2); //set range
      Vin = voltsMeas(config2, slopeAvg);  //take and display measurement 
      //config = 0;
    break;
    case 2: //amps
      digitalWrite(PA4, LOW); //ADCOpto2 (Master volts control)
      digitalWrite(PA5, HIGH); //ADCOpto1 (Master current range)
      
      ampsMult = ampsRange(config2); //set range
      Vin = voltsMeas(4, slopeAvg);  //take and display measurement 
      Vin = (Vin / 1000) / ampsMult; 
    break;
    case 3: //ohms
      ohmsRange(config2); //set range
      Vin = voltsMeas(config2, slopeAvg);  //take and display measurement 
      config = 0;
    break;
  }
  
  //screen setup
  display.clearDisplay();
  display.setTextSize(2);      // Normal 1:1 pixel scale = 3
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //displays on screen (value, number of digits (after decinal?))
  display.print(Vin, 3);
  display.print(" ");
  display.print(ampsMult);

  //displayPostfix(config, config2); //display V / A / Ohms
  display.display();
}



//takes measurement and does convertion to displayable value
double voltsMeas(int config2, double slopeAvg)
{
  //func variable declarations
  double val = 0; //adc readings
  double val2 = 0; //adc readings
  double Vin = 0;
  double refVoltage = 3.000; //needs cal'ed
  int avgNum = 50; //number of readings to be averaged for displayed reading



  //measurements collection for averaging 
  for(int x = 0; x < avgNum; x++) //5000 readings 
  {
    //set adc pin and add measurement to average total
    val += analogRead(PB0);//0 - 65535.00
  }

  // ADC_HandleTypeDef hadc1;
  // hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  // hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  // hadc1.Init.ContinuousConvMode = DISABLE;
  // hadc1.Init.NbrOfConversion = 1;
  // hadc1.Init.DiscontinuousConvMode = DISABLE;
  // hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  // if (HAL_ADC_Init(&hadc1) != HAL_OK)
  // {
  //   Error_Handler();
  // }
  // ADC1 regular channel0 configuration
  // ADC_ChannelConfTypeDef sConfig = {0};
  // sConfig.Channel = ADC_CHANNEL_1;
  // sConfig.Rank = ADC_REGULAR_RANK_1;
  // sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  // if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  // {
  //   Error_Handler();
  // }

  // int yy = 0;

  ///while(yy < 10) //5000 readings 
  ///{
  // HAL_ADC_Start(&hadc1);
  // if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK) {
  //   uint32_t value = HAL_ADC_GetValue(&hadc1);
  //   val2 += value;
  //   yy++;
  // }
  // ///}
  // HAL_ADC_Stop(&hadc1);


  //create the average
  val = val / avgNum;
  Vin = ((val / 65535.00) * refVoltage) * rangeMult;
  //Vin = Vin / 0.88;
  Vin = Vin / slopeAvg; //linear gain error correction


  //Vin = val2; //   / 10;

  //Vin = analogRead(PB0);

  return(Vin); //does this mess up uses where return is not assigned to variable???
}





//opens optos in accordance with what range was selected
//  - needs adc protection (aka overload ability)
void voltsRange(int config2)
{
  //amps optos
  digitalWrite(PA11, HIGH); //10A AOpto1
  digitalWrite(PA10, HIGH); //1A AOpto2
  digitalWrite(PA9, HIGH); //100mA AOpto3
  digitalWrite(PA8, HIGH); //10mA AOpto4
  digitalWrite(PD8, HIGH); //1mA AOpto5
  delay(10); //give time for optos to close

  switch(config2)
  {
    //start on highest range for range sweping 
    case 1: //200V
      rangeMult = 100;
      digitalWrite(PA1, LOW); //ADCOpto5 (200mV)
      digitalWrite(PB4, LOW); //ADCOptoNew (200mV) output
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      delay(10); //give time for optos to close
      digitalWrite(PA0, HIGH); //Vopto1 (Voltage divider input)
      digitalWrite(PA3, HIGH); //ADCOpto3 (2V 200V)
    break;
    case 2: //10V
      rangeMult = 5;
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA1, LOW); //ADCOpto5 (200mV)
      digitalWrite(PB4, LOW); //ADCOptoNew (200mV) output
      digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
      delay(10); //give time for optos to close
      digitalWrite(PA2, HIGH); //ADCOpto4 (10V)
    break;
    case 3: //2V
      rangeMult = 1;
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA1, LOW); //ADCOpto5 (200mV)
      digitalWrite(PB4, LOW); //ADCOptoNew (200mV) output
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      delay(10); //give time for optos to close
      digitalWrite(PA3, HIGH); //ADCOpto3 (2V 200V)
    break;
    case 4: //200mV
      rangeMult = 100;
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
      delay(10); //give time for optos to close
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break; 
  }
}


double ampsRange(int config2)
{
  double mult = 0;
  //voltage optos
  digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
  digitalWrite(PA2, LOW); //ADCOpto4 (10V)
  digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
  delay(10); //give time for optos to close

  switch(config2)
  {

    case 1: //10A
      mult = 0.01; //10mOhms
      digitalWrite(PA10, HIGH); //1A AOpto2
      digitalWrite(PA9, HIGH); //100mA AOpto3
      digitalWrite(PA8, HIGH); //10mA AOpto4
      digitalWrite(PD8, HIGH); //1mA AOpto5
      delay(10); //give time for optos to close

      digitalWrite(PA11, LOW); //10A AOpto1
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break;
    case 2: //1A
      mult = 0.1; //100mOhms
      digitalWrite(PA11, HIGH); //10A AOpto1
      digitalWrite(PA9, HIGH); //100mA AOpto3
      digitalWrite(PA8, HIGH); //10mA AOpto4
      digitalWrite(PD8, HIGH); //1mA AOpto5

      delay(10); //give time for optos to close
      digitalWrite(PA10, LOW); //1A AOpto2
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break;
    case 3: //100mA
      mult = 1; //1 Ohm
      digitalWrite(PA11, HIGH); //10A AOpto1
      digitalWrite(PA10, HIGH); //1A AOpto2
      digitalWrite(PA8, HIGH); //10mA AOpto4
      digitalWrite(PD8, HIGH); //1mA AOpto5

      delay(10); //give time for optos to close
      digitalWrite(PA9, LOW); //100mA AOpto3
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break;
    case 4: //10mA
      mult = 10; //10 Ohms
      digitalWrite(PA11, HIGH); //10A AOpto1
      digitalWrite(PA10, HIGH); //1A AOpto2
      digitalWrite(PA9, HIGH); //100mA AOpto3
      digitalWrite(PD8, HIGH); //1mA AOpto5

      delay(10); //give time for optos to close
      digitalWrite(PA8, LOW); //10mA AOpto4
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break;
    case 5: //1mA
      mult = 100; //100 Ohms
      digitalWrite(PA11, HIGH); //10A AOpto1
      digitalWrite(PA10, HIGH); //1A AOpto2
      digitalWrite(PA9, HIGH); //100mA AOpto3
      digitalWrite(PA8, HIGH); //10mA AOpto4

      delay(10); //give time for optos to close
      digitalWrite(PD8, LOW); //1mA AOpto5
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break;
  }
  return(mult);
}



void ohmsRange(int config2)
{
  digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
  digitalWrite(PA2, LOW); //ADCOpto4 (10V)
  digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
  delay(10); //give time for optos to close
  //comented out for ADC safty
  //digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
  switch(config2)
  {
    //change postfix insted of measurement number??????
    //  - at least at certin range????
    case 1: //100 ohms
      rangeMult = 10;
    break;
    case 2: //1K ohms
      rangeMult = 100;
    break;
    case 3: //10K ohms
      rangeMult = 100;
    break;
    case 4: //100K ohms
      rangeMult = 100;
    break;
  }
}


//displays the function being used (volts, amps, ohms)
void displayPostfix(int config, int config2)
{
  switch(config) 
  {
    case 1: //volts 
      if(config2 == 1)
      {  //200mV
        display.print("mV");
      }
      else
      {
        display.print("V");
      }
      break;
    case 2: //amps
      switch(config2) 
      {
        case 1:
          display.print("mA");
          break;
        case 2:
          display.print("A");
          break;
      }
      break;
    case 3: //ohms
      switch(config2) 
      {
        //could display ohms symbol?? - video shows how
        case 1:
          display.print("R");
          break;
        case 2:
          display.print("kR");
          break;
      }  
      break;
  }
}

