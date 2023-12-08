#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//I2C setup
#include <Wire.h>

#include "screenDisplay.hpp"

extern Adafruit_SSD1306 display; //becasue "display" is defined in screenDisplay but also needs to be defined here, we can use and extern to define it I guess?

//file variable declarations
int config = 0; //tells displayADC which mode is used
int config2 = 0; //tells displayADC which range is used
double rangeMult = 1; //multiplier to acount for range on display


//main function, dictates all ADC function related control
//  - take ADC measurement, display measure screen
void displayADC(int config, int config2, double offset) 
{ 
  //clear display
  display.clearDisplay();


  //volts/amps/ohms setup
  switch(config) 
  {
    case 1:
      //volts 
      voltsRange(config2); //set range
      voltsMeas(config2, offset);  //take and display measurement 

    case 2:
      ampsRange(config2); //set range
      ampsMeas(config2, offset);  //take and display measurement
    case 3:
      ohmsRange(config2); //set range
      ohmsMeas(config2, offset);  //take and display measurement
      break;
  }

  //update the display
  display.display();
}




//opens optos in accordance with what range was selected
//  - needs adc protection (aka overload ability)
void voltsRange(int config2)
{
  switch(config2)
  {
    //start on highest range for range sweping 
    case 1: //200V and 2V
      rangeMult = 100;
      digitalWrite(PA1, LOW);
      digitalWrite(PA3, LOW);
      delay(100);
      //Voltage divider input
      digitalWrite(PA0, HIGH);
      //ADCOpto5
      digitalWrite(PA1, HIGH);
    break;

    case 2: //10V
      rangeMult = 10;
      digitalWrite(PA0, LOW);
      digitalWrite(PA1, LOW);
      digitalWrite(PA2, LOW);
      delay(100);
      //ADCOpto4
      digitalWrite(PA2, HIGH);
    break;

    case 3: //2V
      rangeMult = 1;
      digitalWrite(PA0, LOW);
      digitalWrite(PA1, LOW);
      digitalWrite(PA3, LOW);
      delay(100);
      //ADCOpto3
      digitalWrite(PA3, HIGH);
    break;

    case 4: //200mV
      rangeMult = 100;
      digitalWrite(PA0, LOW);
      digitalWrite(PA2, LOW);
      digitalWrite(PA3, LOW);
      delay(100);
      //ADCOpto5
      digitalWrite(PA1, HIGH);
    break;
   
  }
}

void ampsRange(int config2)
{
  switch(config2)
    case 1: //100mA
    rangeMult = 100;
    digitalWrite(PA0, LOW);
    digitalWrite(PA2, LOW);
    digitalWrite(PA3, LOW);
    delay(100);
    //ADCOpto5
    digitalWrite(PA1, HIGH);
}

void ohmsRange(int config2)
{

}

//takes measurement and does convertion to displayable value
void voltsMeas(int config2, double offset)
{
  //func variable declarations
  double val = 0; //adc readings
  double Vin = 0;
  double refVoltage = 3.000; //needs cal'ed
  int avgNum = 500; //number of readings to be averaged for displayed reading


  //ADCOpto2 master volts control
  digitalWrite(PA4, HIGH);


  //measurements collection for averaging 
  for(int x = 0; x < avgNum; x++) //5000 readings 
  {
    //delay between measuremnets
    //delay(5);

    //set adc pin and add measurement to average total
    val += analogRead(PB0);
   }

   //create the average
   val = val / avgNum;
   Vin = ((val / 65535.00) * refVoltage) * rangeMult + offset;

 
  /* test code
  //set adc pin
  val = analogRead(PB0);
  
  //scale adc reading to usable voltage
  Vin = ((val / 65535.00) * refVoltage) * rangeMult + offset;
  */


  //screen setup
  display.setTextSize(3);      // Normal 1:1 pixel scale = 3
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //displays on screen (value, number of digits (after decinal?))
  display.print(Vin, 4);

}

void ampsMeas(int config2, double offset)
{

}

void ohmsMeas(int config2, double offset)
{

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