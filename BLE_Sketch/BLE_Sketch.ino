#include <Time.h>
#include <RBL_nRF8001.h>
#include <boards.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

/*

Copyright (c) 2012, 2013 RedBearLab

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#include <SPI.h>
#include <EEPROM.h>


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(25, 3, NEO_GRB + NEO_KHZ800);

int command = 0;

int in1 = 0;
int in2 = 0;
int in3 = 0;
int in4 = 0;
int mode = -1;
int lastMinute, lastHour = 0;
LiquidCrystal lcd(10,9,4,2,1,0);
time_t lastFed;
int button1Pin = 5;
int lastR, lastG, lastB, lastBr;

void setup() 
{
  pinMode(button1Pin, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  ble_set_pins(7, 6);

  ble_begin();
  colorWipe(255,0,0,255);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("TankMon");
}

void loop() 
{
  if(ble_available())
  {
    command = ble_read();
    in1 = ble_read();
    in2 = ble_read();
    in3 = ble_read();
    in4 = ble_read();

    //change mode
    if (command == 1) { // color
      mode = 1;
      colorWipe( in1, in2, in3, in4 );
    } else if (command == 0) { //standard
      mode = 0;
      time_t t = now();  
      timeColor(t);  
      
    } else if (command == 2) { // off
      mode = 2;
      colorWipe(0,0,0,100);
    }  else if (command == 255) {
      setTime(in1, in2, 0, 1, 1, 2015);
    }
    
  } else {
    //process modes if no ble
    
    if (mode == 0) {
      time_t t = now(); 
      if (second(t) == 0) {
        timeColor(t);
      }
    } else if (mode == -1) {
      colorWipe(255,0,0,255);
    }
  }

  ble_do_events();

  //write time to display
  time_t t = now();
  if (hour(t) != lastHour || minute(t) != lastMinute) {
    displayTime(1,0,t);
    lastHour = hour(t);
    lastMinute = minute(t);
  }

  //write mode to display
  lcd.setCursor(13,0);
  lcd.print(getMode());
  
  //button input for feeding
  if (digitalRead(button1Pin) == LOW) {
    lastFed = now();
    lcd.setCursor(8,1);
    lcd.print("LF:");
    displayTime(1,11,lastFed);
  }

  //flash for feeding needed
  int lastHr = hour(lastFed);
  int nowHr = hour(t);
  //if more than 14 hour difference 
  if (second(t) % 10 == 0) {
    if (lastHr > nowHr) {
      int diff24 = 24 - lastHr;
      if (diff24 + nowHr > 14) {
        //feed me
        foodLightFlash();
      }
    } else if (nowHr - lastHr > 14) {
      //feed me
      foodLightFlash();
    }
  }
  
  

  
  
}

// Fill the dots one after the other with a color
void colorWipe(int r, int g, int b, int brightness) 
{
  lastR = r;
  lastG = g;
  lastB = b;
  lastBr = brightness;
  if (brightness > 220) {
    brightness = 220;
  }
  r = (int) (((double) brightness/255) * r);
  g = (int) (((double) brightness/255) * g);
  b = (int) (((double) brightness/255) * b);
  
  uint32_t c = strip.Color(r, g, b);
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    
      strip.setPixelColor(i, c);
      strip.show();
  }
}

void timeColor(time_t t) {

  int currentHour = hour(t);
  //night low power
  if (currentHour >= 22 || (currentHour >= 0 && currentHour <= 4)) 
  {
    colorWipe(255,255,190,25);
  }

  //evening/morning
  else if ((currentHour >= 20 && currentHour < 22) || (currentHour > 4 && currentHour <= 6))
  {
    colorWipe(255, 255, 190, 140);
  }
  // daytime
  else
  {
    colorWipe(255,255,190,200);
  }
}

String getMode() {

    if (mode == 0) {
      return "STD";
    } else if (mode == 1) {
      return "COL";
    } else if (mode == 2) {
      return "OFF";
    } else if (mode == -1) {
      return "RBT";
    }else {
      return "OTH";
    }
}

//requires 5 screen positions
void displayTime(int startHeight, int startIndex, time_t t) {
        lcd.setCursor(startIndex,startHeight);
      lcd.print("  ");
      lcd.setCursor(startIndex,startHeight);
      int hr = hour(t);
      if (hr < 10) {
        lcd.print(" ");
        lcd.print(hr);
      } else {
        lcd.print(hr);
      }
      
      lcd.setCursor(startIndex+2,startHeight);
      lcd.print(":");
      lcd.setCursor(startIndex+3,startHeight);
      lcd.print("  ");
      lcd.setCursor(startIndex+3,startHeight);
      int min = minute(t);
      if (min < 10) {
        lcd.print("0");
        lcd.print(min);
      } else {
        lcd.print(min);
      }

}

void foodLightFlash() {
  uint32_t c = strip.Color(255, 0, 0);
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    
      strip.setPixelColor(i, c);
      delay(50);
      strip.show();
  }

  c = strip.Color(0, 0, 255);
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    
      strip.setPixelColor(i, c);
      delay(50);
      strip.show();
  }
  colorWipe(lastR, lastG, lastB, lastBr);
}

