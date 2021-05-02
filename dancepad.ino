#include <Keyboard.h>

#define upThreshold 400
#define rightThreshold 500
#define downThreshold 500
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
#define NUM_STEPS 4
int led_indices[NUM_STEPS] = {RIGHT_LED_INDEX, UP_LED_INDEX, DOWN_LED_INDEX, LEFT_LED_INDEX};

unsigned long startMillis;
unsigned long currentMillis;

void tick() {
  currentMillis = millis();
}

void solidColor(int stepIndex, CRGB color) {
  leds(stepIndex, stepIndex + NUM_LEDS_PER_STEP - 1).fill_solid(color);
  FastLED.show();
}

void carnival(int stepIndex, CRGB color, int interval) {
  bool odd = true;
  if ((currentMillis / interval) % 2 == 0) {
    odd = !odd;
  }
  for (int i = 0; i < NUM_LEDS_PER_STEP; i++) {
    if ((i % 2 == 0) == odd){
      leds[i + stepIndex] = color;
    } else {
      leds[i + stepIndex] = CRGB::Black;
    }
  }
  FastLED.show();
}

void gamer(int stepIndex, int interval) {
  int colorOffset = currentMillis / interval;
  for (int i = 0; i < NUM_LEDS_PER_STEP; i++) {
    leds[i + stepIndex] = CHSV(((i * 255) / NUM_LEDS_PER_STEP) + colorOffset, 255, 255);
  }
  FastLED.show();
}

void swirl(int stepIndex, CRGB color, int interval) {
  int offset = (currentMillis / interval) % NUM_LEDS_PER_STEP;
  for (int i = 0; i < NUM_LEDS_PER_STEP; i++) {
    if (i == offset) {
      leds[stepIndex + i] = color;
    } else {
      leds[stepIndex + i] = CRGB::Black;
    }
  }
  FastLED.show();
}

void rainbowSwirl(int stepIndex, int colorInterval, int movementInterval) {
  int colorOffset = currentMillis / colorInterval;
  int ledOffset = (currentMillis / movementInterval) % NUM_LEDS_PER_STEP;
  for (int i = 0; i < NUM_LEDS_PER_STEP; i++) {
    if (i == ledOffset) {
      leds[stepIndex + i] = CHSV(((i * 255) / NUM_LEDS_PER_STEP) + colorOffset, 255, 255);
    } else {
      leds[stepIndex + i] = CRGB::Black;
    }
  }
  FastLED.show();
}

void setActive(int stepIndex) {
  solidColor(stepIndex, CRGB::White);
}

void setIdle(int stepIndex) {
  gamer(stepIndex, 10);
}

void setup() {
  startMillis = millis();
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  for (int i = 0; i < NUM_STEPS; i++) {
    setIdle(led_indices[i]);
  }
}

void loop() {
  tick();
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
