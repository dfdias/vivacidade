#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
#define CLOCK_PIN 13
#define CHIPSET     WS2811
// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  analogReference(INTERNAL);
    
}

void loop() { 
 int lftlvl=readsignal(A2);
  display(lftlvl); 
}

int readsignal(byte pin){
  int lvl;
  lvl= analogRead(pin);
  return lvl;
}

void display(int lvl){
  if(lvl==0){
    leds[0]=CRGB::Black;
    leds[1]=CRGB::Black;
    leds[2]=CRGB::Black;
    leds[3]=CRGB::Black;
  }
  if (lvl>0 && lvl<256){
    leds[0]=CRGB::Blue;
    leds[1]=CRGB::Black;
    leds[2]=CRGB::Black;
    leds[3]=CRGB::Black;
  }
  else if(lvl>=256 && lvl<512){
    leds[0]=CRGB::Yellow;
    leds[1]=CRGB::Yellow;
    leds[2]=CRGB::Black;
    leds[3]=CRGB::Black;
  }

  else if(lvl>=512 && lvl<768){
    leds[0]=CRGB::Orange;
    leds[1]=CRGB::Orange;
    leds[2]=CRGB::Orange;
    leds[3]=CRGB::Black;
  }
  else
    leds[0]=CRGB::Red;
    leds[1]=CRGB::Red;
    leds[2]=CRGB::Red;
    leds[3]=CRGB::Red;

  FastLED.show();

} 
