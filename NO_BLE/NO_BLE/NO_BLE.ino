#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, 3, NEO_GRB + NEO_KHZ800);

int mode;
int r;
int g;
int b;
int brightness;
int checksum;

int lastR, lastG, lastB, lastBr;

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(255,0,0,255);
  Serial.begin(115200);
  Serial.setTimeout(10);

  mode = 0;
  r = 0;
  g = 0;
  b = 0;
  brightness = 0;
  checksum = 0;
}

void loop() 
{
  if(Serial.available()) {
    int mode_read = Serial.parseInt();
    int r_read = Serial.parseInt();
    int g_read = Serial.parseInt();
    int b_read = Serial.parseInt();
    int brightness_read = Serial.parseInt();
    checksum = Serial.parseInt();

    if(mode_read + r_read + g_read + b_read + brightness_read == checksum) {
      mode = mode_read;
      r = r_read;
      g = g_read;
      b = b_read;
      brightness = brightness_read;
      
      Serial.print(mode_read);
      Serial.print(",");
      Serial.print(r_read);
      Serial.print(",");
      Serial.print(g_read);
      Serial.print(",");
      Serial.print(b_read);
      Serial.print(",");
      Serial.println(brightness_read);
    } else {
      Serial.println("err");
      Serial.print(mode_read);
      Serial.print(",");
      Serial.print(r_read);
      Serial.print(",");
      Serial.print(g_read);
      Serial.print(",");
      Serial.print(b_read);
      Serial.print(",");
      Serial.println(brightness_read);
    }
  }

    if (mode == 0) {
      directControl();
    } else if (mode == 6) {
      colorWipe(255,0,0,255);
    } else if(mode==1) {
      rainbowCycle(10);
    } else if(mode==2) {
      chillMode(10); // Cycles through all colors
    } else if(mode==3) {
      aquaticColors(10);
    } else if(mode==4) {
      hellMode(10);
    } else if(mode==5) {
      vegasMode(10);
    }
  delay(10);
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
      if(Serial.available()) {
        i = strip.numPixels();
      }
  }
}

void directControl() {
  float multiplier = (float) brightness / (float) 255;
  for(int i = 0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(r*multiplier, g*multiplier, b*multiplier));
  }
  strip.show();
}


void foodLightFlash() {
  uint32_t c = strip.Color(255, 0, 0);
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    
      strip.setPixelColor(i, c);
      delay(50);
      strip.show();
      if(Serial.available()) {
        i = strip.numPixels();
      }
  }

  c = strip.Color(0, 0, 255);
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    
      strip.setPixelColor(i, c);
      delay(50);
      strip.show();
      if(Serial.available()) {
        i = strip.numPixels();
      }
  }
  colorWipe(lastR, lastG, lastB, lastBr);
}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);

    // Break if serial transmission
    if(j%8==0 && Serial.available()) {
      j = 256*5;
    }
  }
}

void chillMode(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait*20);
    if(Serial.available()) {
      j = 256*5;
    }
  }
}

void aquaticColors(uint8_t wait) {
  uint16_t i, j, wheelPos; //100-150
  for(j=0; j<100; j++) {
    wheelPos = j;
    if(j>50) {
      wheelPos = 100 - j;
    }
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((100 + wheelPos) & 255));
    }
    strip.show();
    delay(wait*10);
    if(Serial.available()) {
      j = 100;
    }
  }
}

void hellMode(uint8_t wait) {
  uint16_t i, j;
  float wheelPos;

  // Flash red faster and faster
  for(int k = 1000; k>0; k--) {
    int l = sqrt(k);
    for(i=0; i< strip.numPixels(); i++) {
      if(l%2==0) {
        strip.setPixelColor(i, strip.Color(255,0,0));
      } else {
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
    }
    strip.show();
    delay(3);
    if(k>200) {
      if(k%100==0) {
        if(Serial.available()) {
          k = 0;
        }
      }
    }
    
  }
  // Last coule of flashes
  for(int k = 0; k<50; k++) {
     for(i=0; i< strip.numPixels(); i++) {
      if(k%2==0) {
        strip.setPixelColor(i, strip.Color(255,0,0));
      } else {
        strip.setPixelColor(i, strip.Color(0,0,0));
      }
    }
    strip.show();
    if(Serial.available()) {
      k = 50;
    }
    delay(10);
  }

  int timeconst = 5;

  int initialWheelPos = 20;
  int wheelPosDev = 4;
  wheelPos = initialWheelPos;
  float posSlider = wheelPos;

  int rVal, gVal, gRand, bRand;
  float bVal;
  
  rVal = 255;
  gVal = 70;
  bVal = 1;
  // Flicker
  bool keeplooping = true;
  while(keeplooping) {
        gRand = random(-5,5);
        bRand = random(-200,210);
        gVal += gRand;
        bVal += (float) bRand/1500;
        if(gVal>80) {
          gVal = 80;
        } else if(gVal<50) {
          gVal = 50;
        }
        if(bVal>1) {
          bVal = 1;
        } else if(bVal<.3) {
          bVal = .3;
        }
      for(int p=0; p< strip.numPixels(); p++) {
        strip.setPixelColor(p, rVal*bVal, gVal*bVal, 0);
      }
      delay(20);
      strip.show();
      if(Serial.available()) {
        keeplooping = false;
      }
  }
}

void vegasMode(uint8_t wait) {
  int ndx2;
  int r1 = 250;
  int g1 = 0;
  int b1 = 0;
  wait +=6;
  int counter = 0;
  bool keeplooping = true;
  while(keeplooping) {
    if(wait>2) {
      wait--;
    } else {
      counter++;
      if(counter>10) {
        counter = 10;
      }
    }
    for(int ndx = 0; ndx<=strip.numPixels()*2; ndx++) {
      if(ndx>strip.numPixels()) {
        ndx2 = strip.numPixels()*2 - ndx;
      } else {
        ndx2 = ndx;
      }
      for(int i = 0; i<strip.numPixels(); i++){
        if(counter<10) {
          if(i>ndx2-2 && i< ndx2+2) {
            strip.setPixelColor(i, r1/(abs(ndx2-i)+1), g1/(abs(ndx2-i)+1), b1/(abs(ndx2-i)+1));
          } else {
            strip.setPixelColor(i, 0, 0, 0);
          }
        } else {
          strip.setPixelColor(i, r1, g1, b1);
        }
      }
      strip.show();
      if(counter<10) {
        delay(wait*2);
      } else {
        delay(wait*10);
      }
      if(Serial.available()) {
        keeplooping = false;
      }
      
      int stepval = 3;
      if(b1 == 0) {
        if(g1 == 0) {
          if(r1<=255-stepval) {
            r1 += stepval;
          } else {
            g1 += stepval;
            r1 -= stepval;
          }
        } else {
          if(g1<=255-stepval) {
            g1 += stepval;
            if (r1>stepval) {
              r1 -= stepval;
            } else {
              r1 = 0;
            }
          } else {
            b1 += stepval;
            g1 -= stepval;
          }
        }
      } else {
        if(r1 == 0) {
          if(b1<=255-stepval) {
            b1 += stepval;
            if(g1>stepval) {
              g1 -= stepval;
            } else {
              g1 = 0;
            }
          } else {
            r1 += stepval;
            b1 -= stepval;
          }
        } else {
          if(r1<=255-stepval) {
            r1 += stepval;
          }
          if(b1>stepval) {
            b1 -= stepval;
          } else {
            b1 = 0;
          }
        }
      }
    }
  }
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
