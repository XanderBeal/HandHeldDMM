
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//I2C setup
#include <Wire.h>

#include "screenDisplay.hpp"

extern Adafruit_SSD1306 display;

//variable declarations
int sensorPin = PB0;  // input pin for the potentiometer
double val = 0;
double Vin = 0;
int config = 0; //tells displayADC which mode is used
int config2 = 0; //tells displayADC which range is used
double rangeMult = 1; //multiplier to acount for range on display



//take ADC measurement, display measure screen
void displayADC(int config, int config2, double offset) 
{ 
  //clear display
  display.clearDisplay();



  //volts/amps/ohms UI
  switch(config) 
  {
    case 1:
      //volts 
      voltsRange(config2); //set range
      ADCMeas(config2, offset);  //take measurement

    case 2:

    case 3:
      //ohms
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

  
  //int getArrayLength = sizeof(cstr) / sizeof(int);

  // for (int i = 1; i < 5 ; i++)
  // {
  //    display.writeLine(cstr[i - 1]);
  //    display.setCursor(i * 23, 0);
  // }

  display.display();
  //Serial.print("\n");
  //delay(100);
}

void voltsRange(int config2)
{
  switch(config2)
  {
    case 1:
    rangeMult = 100;
    digitalWrite(PA0, LOW);
    digitalWrite(PA2, LOW);
    digitalWrite(PA3, LOW);
    delay(100);
    //ADCOpto5
    digitalWrite(PA1, HIGH);
    break;

    case 2:
    rangeMult = 1;
    digitalWrite(PA0, LOW);
    digitalWrite(PA1, LOW);
    digitalWrite(PA3, LOW);
    delay(100);
    //ADCOpto3
    digitalWrite(PA3, HIGH);
    break;

    case 3:
    rangeMult = 10;
    digitalWrite(PA0, LOW);
    digitalWrite(PA1, LOW);
    digitalWrite(PA2, LOW);
    delay(100);
    //ADCOpto4
    digitalWrite(PA2, HIGH);
    break;

    case 4:
    rangeMult = 100;
    digitalWrite(PA2, LOW);
    digitalWrite(PA3, LOW);
    delay(100);
   //Voltage divider input
    digitalWrite(PA0, HIGH);
    //ADCOpto5
    digitalWrite(PA1, HIGH);
    break;
   
  }
}

void ampsMeas(void)
{

}

void ohmsMeas(void)
{

}

void ADCMeas(int config2, double offset)
{
  double refVoltage = 3.000; //needs cal'ed

  //averaging code
  //for(int x = 0; x < 5000; x++) 
  //{
  //  val += analogRead(sensorPin);
  // }

  //val = val / 5000.0;

  //only volts configured
  //ADCOpto2 master volts control
  digitalWrite(PA4, HIGH);


  
  val = analogRead(sensorPin);
  
  //scale adc reading to usable voltage
  Vin = ((val / 65535.00) * refVoltage) * rangeMult + offset;

  //char cstr[4] ;
      //Serial.print(Vin, 5);
      //Serial.print("\n");
  display.setTextSize(3);      // Normal 1:1 pixel scale = 3
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //prints (value, number of digits of value)
  display.print(Vin);

}

void postfix(int config, int config2)
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