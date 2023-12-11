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
double val2 = 0; //random var
int calRange = 0; //sets cal range for all functions in this file
int calRangeTemp = 1; //sets cal range for all functions in this file



double calFunc(void)
{
    val2 = 0.00;

    return(val2);
}



void slopeCalc(void)
{
    //intrupt creation
    attachInterrupt(digitalPinToInterrupt(PC15), InteruptCal1, LOW);

    double slopeData[5];
    double Vin = 0;

    while(calRange < 5)
    {

        switch(calRangeTemp)
        {   case 1:
            display.setTextSize(2);      // Normal 1:1 pixel scale = 3
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.clearDisplay();
            display.print("Set volts = ");
            display.print("0.500000");
            display.display();
            break;
            case 2:
            //measure input voltage
            Vin = voltsMeas(2,0); //sets range to 2V and offset to zero (for now)
            delay(1000);
            //Input voltage = 0.5V
            slopeData[0] = Vin - 0.50000;
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Set volts = ");
            display.print("1.00000");
            display.display();
            //calRangeTemp = 0;
            break;
            case 3:
            //measure input voltage
            Vin = voltsMeas(2,0); //sets range to 2V and offset to zero (for now)
            delay(1000);
            //Input voltage = 1V
            slopeData[1] = Vin - 1.00000;
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Set volts = ");
            display.print("1.50000");
            display.display();
            //calRangeTemp = 0;
            break;
            case 4:
            //measure input voltage
            Vin = voltsMeas(2,0); //sets range to 2V and offset to zero (for now)
            delay(1000);
            //Input voltage = 1.5V
            slopeData[2] = Vin - 1.50000;
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Set volts = ");
            display.print("2.00000");
            display.display();
            //calRangeTemp = 0;
            break;
            case 5:
            //measure input voltage
            Vin = voltsMeas(2,0); //sets range to 2V and offset to zero (for now)
            delay(1000);
            //Input voltage = 2V
            slopeData[3] = Vin - 2.00000;
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Set volts = ");
            display.print("2.50000");
            display.display();
            //calRangeTemp = 0;
            break;
            case 6: //Hint: 2V range can measure up to 3V due to 3V ref
            //measure input voltage
            Vin = voltsMeas(2,0); //sets range to 2V and offset to zero (for now)
            delay(1000);
            //Input voltage = 2.5V
            slopeData[4] = Vin - 2.50000;
            display.clearDisplay();
            display.setCursor(0, 0);
            display.print("Cal Compleate");
            display.display();
            //calRangeTemp = 0;
            break;
        }
    }
}

//cycle through cal ranges and display needed input voltage for range cal
void InteruptCal1()
{
  if(calRange < 5 && calRange != 0)
  {
    calRange = calRange + 1;
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale = 3
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Cal Range");
    display.print("Compleate");
    display.display();
    calRangeTemp = calRange + 1;
  }
  else
  {
    calRange = 1;
    calRangeTemp = calRange + 1;
  }
 
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
