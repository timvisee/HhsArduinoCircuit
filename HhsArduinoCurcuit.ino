#include <Arduino.h>

/**
 * Defines the number of milliseconds when a button state has changed to prevent jitter (/bouncing).
 */
const BUTTON_JITTER_DELAY = 50;

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
 * Stores the current state of the shift button.
 */
bool shiftButtonState = false;

/**
 * Stores the current state of the toggle button.
 */
bool toggleButtonState = false;

/**
 * Stores the index of the seleted input LED.
 */
int selectedLed = 0;

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
    // Define a variable for the new shift and toggle button states
    bool shiftButton = false;
    bool toggleButton = false;

    // Check whether the shift button is pressed
    if(digitalRead(BUTTON_SHIFT_PIN) == HIGH)
        shiftButton = true;

    // Check whether the toggle button is pressed
    if(digitalRead(BUTTON_TOGGLE_PIN) == HIGH)
        toggleButton = true;

    // Check whether the state of the shift button has changed
    if(shiftButton != shiftButtonState) {


        // Wait for a little while to prevent button jitter (/bouncing)
        delay(BUTTON_JITTER_DELAY);
    }

    // Check whether the state of the toggle button has changed
    if(toggleButton != toggleButtonState) {


        // Wait for a little while to prevent button jitter (/bouncing)
        delay(BUTTON_JITTER_DELAY);
    }

    // Update the LEDs
    updateLeds();
}

/**
 * Shift the selected LED by one place.
 */
void shiftSelectedLed() {
    // Increase the index
    selectedLed++;

    // If the new index is out of bound, change it to the first LED index again
    if(selectedLed >= LED_INPUT_COUNT)
        selectedLed = 0;
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