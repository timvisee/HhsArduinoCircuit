#include <Arduino.h>

/**
 * The number of LEDs available as input.
 */
const LED_INPUT_COUNT = 4;

/**
 * The pin's of the input LEDs;
 */
const int LED_INPUT_PIN[LED_INPUT_COUNT] = {2, 3, 4, 5};

/**
 * The pin of the output LED.
 */
const int LED_OUTPUT_PIN = 12;

/**
 * Defines the pin of the button that is used to shift the LED that should be toggled.
 */
const int BUTTON_SHIFT_PIN = 1;

/**
 * Defines the pin of the button that is used to toggle the selected LED.
 */
const int BUTTON_TOGGLE_PIN = 2;

/**
 * Called once on setup.
 */
void setup() {
    // Set up the input LED pins
    for(int i = 0; i < LED_INPUT_COUNT; i++)
        pinMode(LED_INPUT_PIN[i], OUTPUT);

    // Set up the output LED pin
    pinMode(LED_OUTPUT_PIN, OUTPUT);

    // Set up the button pins
    pinMode(BUTTON_SHIFT_PIN, INTPUT);
    pinMode(BUTTON_TOGGLE_PIN, INPUT);
}

/**
 * Called each loop.
 */
void loop() {

}