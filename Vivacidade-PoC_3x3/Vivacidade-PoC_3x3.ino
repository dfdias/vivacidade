#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 40

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 6
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	FastLED.addLeds<NEOPIXEL,DATA_PIN>(leds, NUM_LEDS);
}

void setup() {
  FastLED.addLeds<NEOPIXEL, 4>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 7>(leds, NUM_LEDS_PER_STRIP);
}
