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
    
    display.print("Home Screen");
    display.print("\n");
    display.print("Select a Range");

    display.display();


   
    
}