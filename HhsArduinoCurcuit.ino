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
 * An array that specifies the state of each input LED.
 */
bool LED_STATES[LED_INPUT_COUNT] = {false, false, false, false};

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
    // Update the LEDs
    updateLeds();
}

/**
 * Update the visual state of each LED.
 */
void updateLeds() {
    // Loop through all input LEDs and update their visual state
    for(int i = 0; i < LED_INPUT_COUNT; i++) {
        // Determine whether the new state should be low or high
        int newState = LOW;
        if(LED_STATES[i])
            newState = HIGH;

        // Write the state to the LED pin
        digitalWrite(LED_INPUT_PIN, newState);
    }
}