#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "measure.hpp"
#include "cal.hpp"
//I2C setup
#include <Wire.h>
//display setup
#include "screenDisplay.hpp"
extern Adafruit_SSD1306 display; //becasue "display" is defined in screenDisplay but also needs to be defined here, we can use and extern to define it I guess?

int i = 0; //loop control variable

//slope averaging function
double slopeCalc() 
{
  //attachInterrupt(digitalPinToInterrupt(PC15), InteruptCal1, FALLING);
  double points[5][2];
  double totalSlope = 0.0; //all calculated slopes, pre-average
  //int length = sizeof(points) / sizeof(points[0]);
  double deltaX = 0; //math variable
  double deltaY = 0; //math variable
  double avgSlope = 0.0; //final calculated a

  // display.clearDisplay();
  // display.setCursor(0, 0);
  // display.print("Set volts = ");
  // display.print("0.500000");
  // display.display();
  // while(digitalRead(PC15) == HIGH)
  // {

  // }
  voltsRange(3);
  //gathers 5 data points to create the slope average from
  for (int x = 0; x < 5; x++)
  {
  
    //filling points array
    points[x][0] = 0.5 * (x + 1); //voltage value
    points[x][1] = voltsMeas(3,1.0); //assignes collected avgerage voltage value for 2 v range to array
    //display next voltage 
    display.clearDisplay();
    delay(50);
    display.setCursor(0, 0);
    display.print("Set volts = ");
    display.print(points[x][1]);
    display.display();
    //delay for button input
    while(digitalRead(PC15) == HIGH)
    {
    }
    delay(1000);
  }



  //itterates through the points array and calculates the slop between each adjacent points in the array
  for (int i = 0; i < 5 - 2; i++) {
    deltaX = points[i + 1][0] - points[i][0];
    deltaY = points[i + 1][1] - points[i][1];
    if (deltaX != 0) {
      totalSlope += deltaY / deltaX;
    }
  }
  //creates final average slope
  avgSlope = totalSlope / (5 - 1);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Cal value:");
  display.print("\n");
  display.print(avgSlope);
  display.display();

  return (avgSlope);
  }

//cycle through cal ranges and display needed input voltage for range cal
void InteruptCal1()
{
 
}
void InteruptCal2()
{
    
}
void InteruptCal3()
{
    
}


/* todo
- linear gain error correction
    - voltage scaling steps (every 500mV)
- zero offest detection and correction
- measuremnet stability especialy 1-100 mv range on 2V range
    - 2V range, 3V ref, 65000 ADC values, ideal = 50uV accuracy (no noise)




Syncing an external or internal power supply for finding linear gain error corection formula
*/
