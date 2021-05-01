#include <Keyboard.h>

#define upThreshold 400
#define rightThreshold 500
#define downThreshold 600
#define leftThreshold 500

#define leftPin A0
#define downPin A1
#define upPin A2
#define rightPin A3

int upReading;
int rightReading;
int downReading;
int leftReading;

#include <FastLED.h>
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
#define NUM_LEDS 48
#define NUM_LEDS_PER_STEP 12
#define DATA_PIN 5
CRGBArray<NUM_LEDS> leds;
#define RIGHT_LED_INDEX 0
#define DOWN_LED_INDEX RIGHT_LED_INDEX + NUM_LEDS_PER_STEP
#define LEFT_LED_INDEX DOWN_LED_INDEX + NUM_LEDS_PER_STEP
#define UP_LED_INDEX LEFT_LED_INDEX + NUM_LEDS_PER_STEP
int led_indices[4] = {RIGHT_LED_INDEX, UP_LED_INDEX, DOWN_LED_INDEX, LEFT_LED_INDEX};

void solidColor(int offset, CRGB color) {
  for(int i = 0; i < NUM_LEDS_PER_STEP; i++) {
    leds[i + offset] = color;
  }
  FastLED.show();
}

void setActive(int offset) {
  solidColor(offset, CRGB::White);
}

void setIdle(int offset) {
  solidColor(offset, CRGB::DeepPink);
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  for (int i = 0; i < 4; i++) {
    setIdle(led_indices[i]);
  }
}

void loop() {
  upReading = analogRead(upPin);
  rightReading = analogRead(rightPin);
  downReading = analogRead(downPin);
  leftReading = analogRead(leftPin);

  Serial.print("up reading = ");
  Serial.println(upReading);
  Serial.print("right reading = ");
  Serial.println(rightReading);
  Serial.print("down reading = ");
  Serial.println(downReading);
  Serial.print("left reading = ");
  Serial.println(leftReading);
  
  if (upReading > upThreshold) {
    Keyboard.press(KEY_UP_ARROW);
    setActive(UP_LED_INDEX);
  } else {
    Keyboard.release(KEY_UP_ARROW);
    setIdle(UP_LED_INDEX);
  }
  
  if (rightReading > rightThreshold) {
    Keyboard.press(KEY_RIGHT_ARROW);
    setActive(RIGHT_LED_INDEX);
  } else {
    Keyboard.release(KEY_RIGHT_ARROW);
    setIdle(RIGHT_LED_INDEX);
  }
  
  if (downReading > downThreshold) {
    Keyboard.press(KEY_DOWN_ARROW);
    setActive(DOWN_LED_INDEX);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
    setIdle(DOWN_LED_INDEX);
  }
  
  if (leftReading > leftThreshold) {
    Keyboard.press(KEY_LEFT_ARROW);
    setActive(LEFT_LED_INDEX);
  } else {
    Keyboard.release(KEY_LEFT_ARROW);
    setIdle(LEFT_LED_INDEX);
  }
}
