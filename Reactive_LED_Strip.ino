#include <FastLED.h>

#define NUM_LEDS 100
#define LED_PIN 13
#define minVal 6450
#define maxVal 7000
int reads = 32;
CRGB leds[NUM_LEDS];

#define soundPin A5
#define ctrlPin A0

float soundVal = 0;

void setup () {
  Serial.begin (9600);
  
  pinMode(soundPin, INPUT);
  pinMode(ctrlPin, INPUT);
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
}

void loop () {
  soundVal = 0;
  
  for (int i = 0; i < reads; i = i + 1) {
    soundVal += 1000 * analogRead (soundPin);
  }
  soundVal = soundVal / reads;
  
  //int led_cnt = max(min(fscale(minVal, maxVal, 0, NUM_LEDS - 1, soundVal, -3), NUM_LEDS - 1), 0);
  int led_cnt = max(min(map(soundVal, minVal, maxVal, 0, NUM_LEDS - 1), NUM_LEDS - 1), 0);
  
  Serial.print(soundVal);
  Serial.print(", ");
  Serial.println(led_cnt);

  for (int i = 0; i < led_cnt; i = i + 1) {
      leds[i] = CRGB::Green;
  }
  
  for (int i = led_cnt; i < NUM_LEDS - 1; i = i + 1) {
    leds[i] = CRGB::Black;
  }
  
  FastLED.show();
  delay(5);
}

float fscale( float originalMin, float originalMax, float newBegin, float
              newEnd, float inputValue, float curve) {

  float OriginalRange = 0;
  float NewRange = 0;
  float zeroRefCurVal = 0;
  float normalizedCurVal = 0;
  float rangedValue = 0;
  boolean invFlag = 0;

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  }
  else
  {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;

  }
  else     // invert the ranges
  {
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}
