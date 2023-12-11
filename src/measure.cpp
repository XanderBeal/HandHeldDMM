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
      //config = 0;
    break;
    case 2:
      config2 = 1;
      ampsRange(config2); //set range
      config2 = 4;
      voltsMeas(config2, offset);  //take and display measurement 
      //config = 0;
    break;
    case 3:
      ohmsRange(config2); //set range
      voltsMeas(config2, offset);  //take and display measurement 
      config = 0;
    break;
  }
  //update the display
  display.display();
}




//takes measurement and does convertion to displayable value
double voltsMeas(int config2, double offset)
{
  //func variable declarations
  double val = 0; //adc readings
  double val2 = 0; //adc readings
  double Vin = 0;
  double refVoltage = 3.000; //needs cal'ed
  int avgNum = 50; //number of readings to be averaged for displayed reading
  //Master volts control
  digitalWrite(PA4, HIGH); //ADCOpto2 (input to buffer)
  display.clearDisplay();
  //measurements collection for averaging 
  for(int x = 0; x < avgNum; x++) //5000 readings 
  {
    //delay between measuremnets
    //delay(5);
    //set adc pin and add measurement to average total
    val += analogRead(PB0)* 1.173125;//linear offset correction
   }
  //create the average
  val2 = val / avgNum;
  Vin = ((val2 / 65535.00) * refVoltage) * rangeMult + offset;
  //Vin = Vin * 1.173125; //linear offset correction
  //screen setup
  display.setTextSize(2);      // Normal 1:1 pixel scale = 3
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  //displays on screen (value, number of digits (after decinal?))
  display.print(Vin, 4);
  display.print(" ");
  display.print(config2);
  //display.print(" ");
  //displayPostfix(config, config2); //display V / A / Ohms
  display.display();

  return(Vin); //does this mess up uses where return is not assigned to variable???
}

void ampsMeas(int config2, double offset)
{

}

void ohmsMeas(int config2, double offset)
{

}





//opens optos in accordance with what range was selected
//  - needs adc protection (aka overload ability)
void voltsRange(int config2)
{
  switch(config2)
  {
    //start on highest range for range sweping 
    case 1: //200V
      rangeMult = 100;
      digitalWrite(PA10, LOW); //ADCOpto1 (10A)
      digitalWrite(PA1, LOW); //ADCOpto5 (200mV)
      digitalWrite(PB4, LOW); //ADCOptoNew (200mV) output
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      delay(10); //give time for optos to close
      digitalWrite(PA0, HIGH); //Vopto1 (Voltage divider input)
      digitalWrite(PA3, HIGH); //ADCOpto3 (2V 200V)
    break;
    case 2: //10V
      rangeMult = 5;
      digitalWrite(PA10, LOW); //ADCOpto1 (10A)
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA1, LOW); //ADCOpto5 (200mV)
      digitalWrite(PB4, LOW); //ADCOptoNew (200mV) output
      digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
      delay(10); //give time for optos to close
      digitalWrite(PA2, HIGH); //ADCOpto4 (10V)
    break;
    case 3: //2V
      rangeMult = 1;
      digitalWrite(PA10, LOW); //ADCOpto1 (10A)
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA1, LOW); //ADCOpto5 (200mV)
      digitalWrite(PB4, LOW); //ADCOptoNew (200mV) output
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      delay(10); //give time for optos to close
      digitalWrite(PA3, HIGH); //ADCOpto3 (2V 200V)
    break;
    case 4: //200mV
      rangeMult = 100;
      digitalWrite(PA10, LOW); //ADCOpto1 (10A)
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
      delay(10); //give time for optos to close
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break; 
  }
}


void ampsRange(int config2)
{
  switch(config2)
  {
    case 1: //100mA
      rangeMult = 100;
      digitalWrite(PA0, LOW); //Vopto1 (Voltage divider input)
      digitalWrite(PA2, LOW); //ADCOpto4 (10V)
      digitalWrite(PA3, LOW); //ADCOpto3 (2V 200V)
      delay(10); //give time for optos to close
      digitalWrite(PA10, HIGH); //ADCOpto1 (10A)
      digitalWrite(PA1, HIGH); //ADCOpto5 (200mV)
      digitalWrite(PB4, HIGH); //ADCOptoNew (200mV) output
    break;
  }
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

