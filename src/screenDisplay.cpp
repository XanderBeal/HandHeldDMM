#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//I2C setup
#include <Wire.h>
//screen config
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "measure.hpp"
#include "cal.hpp"
void InteruptBoot1(void);
void InteruptBoot2(void);
int choice2 = 0; //var for boot screen decition
int choice1 = 1;

void bootScreen()
{
    //display setup      
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    { 
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;){
        digitalWrite(PA15, HIGH);
        delay(1000);
        digitalWrite(PA15, LOW);
        delay(100);
    } // Don't proceed, loop forever
    }
}

void calScreen()
{
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale = 3
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print("CalScreen:");
    display.print("\n");
    display.print("Volts=Cal");
    display.print("\n");
    display.print("Amps=Meas");

    attachInterrupt(digitalPinToInterrupt(PC13), InteruptBoot1, FALLING); //triggers interupt function if PC13 goes low (triggers on falling edge)
    attachInterrupt(digitalPinToInterrupt(PC14), InteruptBoot2, FALLING);
    
    while(choice1 = 1)
    {
        if(choice2 = 1)
        {
            display.clearDisplay();
            //slopeCalc();
            choice1 = 0;
        }
        else if(choice2 = 2)
        {
            display.clearDisplay();
            choice1 = 0;
        }
        
    }
    
}

void InteruptBoot1()
{
  choice2 = 1;
}
void InteruptBoot2()
{
  choice2 = 2;
}


/*
void bootScreen(void)
{
    //display setup      
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    { 
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;){
        digitalWrite(PA15, HIGH);
        delay(1000);
        digitalWrite(PA15, LOW);
        delay(100);
    } // Don't proceed, loop forever
     }
    display.clearDisplay();
    display.setTextSize(2);      // Normal 1:1 pixel scale = 3
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
    display.print("HomeScreen");
    display.print("\n");
    display.print("\n");
    display.print("Select a");
    display.print("\n");
    display.print("Range");
    display.display();
}
*/