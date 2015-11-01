#include <Time.h>

#include <RBL_nRF8001.h>


#include <boards.h>




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

void setup() 
{
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  ble_set_pins(7, 6);

  ble_begin();
  colorWipe(255,0,0,255);
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





  
  
}

// Fill the dots one after the other with a color
void colorWipe(int r, int g, int b, int brightness) 
{
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
  if ((hour(t) >= 2 && hour(t) < 6) || (hour(t) >= 22 && hour(t) <= 22)) {
    //late/early
    colorWipe(255,255,255,128);
  } else if ((hour(t) < 2 && hour(t) >= 0) || (hour(t) > 22 && hour(t) <= 24)) {
    //night
    colorWipe(255,255,255,25);
  } else {
    //day
    colorWipe(255,255,255,255);
  }
  

}

