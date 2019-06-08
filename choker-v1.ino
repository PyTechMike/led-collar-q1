#include <Adafruit_NeoPixel.h> 
    
    
// Define
#define NUM_LEDS 30 //60LED per strip
#define DATA_PIN 3 //output pin on ATTiny85
#define BTN_PIN 2 //input pin on ATTiny85
#define BTN_DELAY 250 //add delay for debounce
#define NUM_PATTERNS 6 //patterns avail
#define CTR_THRESH 16

// Init Vars
uint8_t j = 0;
uint8_t pattern=1;
uint8_t buttonState=0;
uint8_t lastPix=0; 
uint8_t myPix=0;
uint8_t direction=1;
uint8_t counter=0;
uint8_t colors[3];
uint32_t setColor=0;
unsigned long mark;

// Start Strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    //pinMode(BTN_PIN, INPUT);     
    randomSeed(BTN_PIN);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(170);
}

void loop() {
    // if button pressed, advance, set mark
    chkBtn(digitalRead(BTN_PIN));
    if (pattern > NUM_PATTERNS) { pattern = 1; }
    pickPattern(pattern);
    if (direction == 1) { j++;  } else {  j--; }

    if (j > 254) { direction = 0; }
    if (j < 1) { direction = 1; }   
   
}

/* pick a pattern */
void pickPattern(uint8_t var) {
      switch (var) {
        case 1:
          wavey(200,240,0.06,0);
        break;
        case 2:
          rainbow(10);
          counter++;
        break;
        case 3:
          colorWipeCenter(strip.Color(255,0,0),100);
          colorWipeCenter(strip.Color(0,0,255),100);
        break;
        case 4:
          strip.clear();
          fadeEveOdd(500,1,20);
        break;
        case 5:
          wavey(0,350,0.06,00);
        break;
        case 6:
          strip.clear();
          fadeEveOdd(0,1,20);
        break;
//        case 5:
//          rainbowCycle(20);
//        break;
//        case 6:
//
//        break;
//        case 7:
//           // color wipe from center YES
//           
//        break;
//        case 8:
//           
//        break;
//        case 9:
//          // fade even or odd YES
//          // 0-359 Hue value, even/odd, delay
//          
//        break;
//        case 10:
//          // show rainbow  YES
//          
//         break; 
//     
//        case 11:
////        // show rainbow
////          theaterChaseRainbow(50);
//         break; 
////     
//        case 12:
////          rainbowCycle2(20);
//         break;
      }
}

/* check button state */
boolean chkBtn(byte buttonState) {
   if (buttonState == HIGH && (millis() - mark) > BTN_DELAY) {
       j = 0;
       mark = millis();
       pattern++;
       return true;
    } 
    else { return false; }
}

void colorFirefly(byte wait) {
        if(myPix != lastPix) {
          if(counter<CTR_THRESH) {
            float colorV = sin((6.28/30)*(float)(counter)) *255;
            HSVtoRGB((359/CTR_THRESH)*counter, 255, colorV, colors);
            strip.setPixelColor(myPix, colors[0], colors[1], colors[2]);
           strip.show();
           delay(wait);
          } else {
            lastPix=myPix;
            counter=0;
            colorFast(0,0);
          }
        } else {
          myPix=random(0,strip.numPixels());
        }

}

// Fill the dots one after the other with a color
// Modified from Neopixel sketch to break on button press

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      if(chkBtn(digitalRead(BTN_PIN))) { break; }
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// color wipe from center
void colorWipeCenter(uint32_t c, uint8_t wait) {
  uint8_t mid=strip.numPixels()/2;
  strip.setPixelColor(mid,c);
  for(uint16_t i=0; i<=strip.numPixels()/2; i++) {
      if(chkBtn(digitalRead(BTN_PIN))) { break; }
      strip.setPixelColor(mid+i, c);
      strip.setPixelColor(mid-i, c);
      strip.show();
      delay(wait);
  }
}

// fast version 
void colorFast(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
    strip.show();
    delay(wait);
}

// Rainbow Cycle, modified from Neopixel sketch to break on button press
void rainbowCycle(uint8_t wait) {
    uint16_t i;

    //  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

void rainbow(uint8_t wait) {
    uint16_t i;

    //for(j=0; j<256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

// scanner

void scanner(uint32_t c,uint8_t wait) {
        for(byte i=0; i< strip.numPixels(); i++) {
            if(chkBtn(digitalRead(BTN_PIN))) { break; }

            colorFast(0,0);
            strip.setPixelColor(i,c);
            strip.show();
            delay(wait);
        }
        for(byte i=strip.numPixels(); i>0; i--) {
           if(chkBtn(digitalRead(BTN_PIN))) { break; }
            colorFast(0,0);
            strip.setPixelColor(i,c);
            strip.show();
            delay(wait);
        }    
}

// scanner to midpoint
void bounceInOut(uint8_t num, uint8_t start,uint8_t wait) {
  colorFast(0,0);
  uint8_t color=200;
  for(byte q=0; q < num; q++) {
    if(strip.numPixels()-start >= 0 && start < NUM_LEDS) {
          strip.setPixelColor(start+q, strip.Color(0,color,0));
          strip.setPixelColor(strip.numPixels()-start-q, strip.Color(0,color,0));
      }   
    color=round(color/2.0);
    strip.show();
    delay(wait);
  }
  if(counter > strip.numPixels()) { counter=0; }
}

void fadeEveOdd(byte c1,byte rem,uint8_t wait) {
              for(byte j=0; j < CTR_THRESH; j++) {
                      for(byte i=0; i< strip.numPixels(); i++) {
                        if(i % 2== rem) {
                           HSVtoRGB(c1,255,(255/CTR_THRESH)*j,colors);
                           strip.setPixelColor(i,colors[0],colors[1],colors[2]);
                         }
                      }           
                      if(chkBtn(digitalRead(BTN_PIN))) { break; }
                      strip.show();
                      delay(wait);
                }
                for(byte j=CTR_THRESH; j >= 0; j--) {
                      for(byte i=0; i< strip.numPixels(); i++) {
                        if(i % 2== rem) {
                           HSVtoRGB(c1,255,(255/CTR_THRESH)*j,colors);
                           strip.setPixelColor(i,colors[0],colors[1],colors[2]);
                         }
                      }             
                     if(chkBtn(digitalRead(BTN_PIN))) { break; }
                      strip.show();
                      delay(wait);
                } 
}

// twinkle random number of pixels
void twinkleRand(byte num,uint32_t c,uint32_t bg,int wait) {
  // set background
   colorFast(bg,0);
   // for each num
   for (byte i=0; i<num; i++) {
     strip.setPixelColor(random(strip.numPixels()),c);
   }
  strip.show();
  delay(wait);
}

// sine wave, low (0-359),high (0-359), rate of change, wait
void wavey(byte low,byte high,float rt,uint8_t wait) {
  float in,out;
  byte diff=high-low;
  byte step = diff/strip.numPixels();
  for (in = 0; in < 6.283; in = in + rt) {
       for(byte i=0; i< strip.numPixels(); i++) {
           out=sin(in+i*(6.283/strip.numPixels())) * diff + low;
           HSVtoRGB(out,255,255,colors);
           strip.setPixelColor(i,colors[0],colors[1],colors[2]);
       }
           strip.show();
           delay(wait);
           if(chkBtn(digitalRead(BTN_PIN))) { break; }
  }
}

// sine wave, intensity
void waveIntensity(float rt,uint8_t wait) {
  float in,level;
  for (in = 0; in < 6.283; in = in + rt) {
       for(byte i=0; i< strip.numPixels(); i++) {
         // sine wave, 3 offset waves make a rainbow!
        level = sin(i+in) * 127 + 128;
        // set color level 
        strip.setPixelColor(i,(byte)level,0,0);
       }
           strip.show();
           delay(wait);
           if(chkBtn(digitalRead(BTN_PIN))) { break; }
  }
}

// helpers 

uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}


// HSV to RGB colors
// hue: 0-359, sat: 0-255, val (lightness): 0-255
// adapted from http://funkboxing.com/wordpress/?p=1366
void HSVtoRGB(byte hue, byte sat, byte val, uint8_t * colors) {
    int r, g, b, base;
    if (sat == 0) { // Achromatic color (gray).
        colors[0] = val;
        colors[1] = val;
        colors[2] = val;
    } else {
        base = ((255 - sat) * val) >> 8;
        switch (hue / 60) {
        case 0:
            colors[0] = val;
            colors[1] = (((val - base) * hue) / 60) + base;
            colors[2] = base;
            break;
        case 1:
            colors[0] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[1] = val;
            colors[2] = base;
            break;
        case 2:
            colors[0] = base;
            colors[1] = val;
            colors[2] = (((val - base) * (hue % 60)) / 60) + base;
            break;
        case 3:
            colors[0] = base;
            colors[1] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[2] = val;
            break;
        case 4:
            colors[0] = (((val - base) * (hue % 60)) / 60) + base;
            colors[1] = base;
            colors[2] = val;
            break;
        case 5:
            colors[0] = val;
            colors[1] = base;
            colors[2] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            break;
        }

    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (byte j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (byte q=0; q < 3; q++) {
        for (byte i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (byte i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle2(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
