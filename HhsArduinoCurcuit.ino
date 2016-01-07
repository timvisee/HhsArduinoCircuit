#include <Arduino.h>

/**
 * Defines the number of milliseconds when a button state has changed to prevent jitter (/bouncing).
 */
const int BUTTON_JITTER_DELAY = 50;

/**
 * Defines the number of milliseconds to show the new state of a LED before making it dimmed again.
 */
const int LED_TOGGLE_SHOW_DURATION = 1000;

/**
 * Defines how bright the selected or dimmed LED will be, from 0 to 255.
 */
const int LED_SELECTED_BRIGHTNESS = 80;

/**
 * The number of LEDs available as input.
 */
const int LED_INPUT_COUNT = 4;

/**
 * The pin's of the input LEDs;
 */
const int LED_INPUT_PIN[LED_INPUT_COUNT] = {5, 6, 9, 10};

/**
 * The pin of the output LED.
 */
const int LED_OUTPUT_PIN = 12;

/**
 * Defines the pin of the button that is used to shift the LED that should be toggled.
 */
const int BUTTON_SHIFT_PIN = 7;

/**
 * Defines the pin of the button that is used to toggle the selected LED.
 */
const int BUTTON_TOGGLE_PIN = 8;

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
 * Stores the index of the selected input LED.
 */
int selectedLed = 0;

/**
 * Called once on setup.
 */
void setup() {
    // Set up the input LED pins
    for(int i = 0; i < LED_INPUT_COUNT; i++) {
        pinMode(LED_INPUT_PIN[i], OUTPUT);
    }

    // Set up the output LED pin
    pinMode(LED_OUTPUT_PIN, OUTPUT);

    // Set up the button pins
    pinMode(BUTTON_SHIFT_PIN, INPUT);
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
    if(digitalRead(BUTTON_SHIFT_PIN) == HIGH) {
        shiftButton = true;
    }

    // Check whether the toggle button is pressed
    if(digitalRead(BUTTON_TOGGLE_PIN) == HIGH) {
        toggleButton = true;
    }

    // Check whether the state of the shift button has changed
    if(shiftButton != shiftButtonState) {
        // Shift the selected LED
        shiftSelectedLed();

        // Wait for a little while to prevent button jitter (/bouncing)
        delay(BUTTON_JITTER_DELAY);
    }

    // Check whether the state of the toggle button has changed
    if(toggleButton != toggleButtonState) {
        // Toggle the selected LED
        toggleSelectedLed();

        // Wait for a little while to prevent button jitter (/bouncing)
        delay(BUTTON_JITTER_DELAY);
    }

    // Update the state of both buttons in the variables
    shiftButtonState = shiftButton;
    toggleButtonState = toggleButton;

    // Do the "circuit" logic
    doLogic();

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
    if(selectedLed >= LED_INPUT_COUNT) {
        selectedLed = 0;
    }
}

/**
 * Toggle the selected LED.
 */
void toggleSelectedLed() {
    // Toggle the state of the selected LED
    LED_STATES[selectedLed] = !LED_STATES[selectedLed];

    // Determine whether the new state is LOW or HIGH
    int newLedState = LOW;
    if(LED_STATES[selectedLed]) {
        newLedState = HIGH;
    }

    // Write the new state to the LED
    digitalWrite(LED_INPUT_PIN[selectedLed], newLedState);

    // Do the program logic
    doLogic();

    // Wait for a little before updating all LEDs again, because this will make the selected LED dimmed
    delay(LED_TOGGLE_SHOW_DURATION);

    // Update the LED states
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
        if(LED_STATES[i]) {
            newState = HIGH;
        }

        // Write the state to the LED pin
        digitalWrite(LED_INPUT_PIN[i], newState);
    }

    // Make the selected LED dimmed
    analogWrite(LED_INPUT_PIN[selectedLed], LED_SELECTED_BRIGHTNESS);
}

/**
 * Do the "circuit" logic with the current button states.
 */
void doLogic() {
    // Define some variables with their states for the inputs
    bool i1 = LED_STATES[0];
    bool i2 = LED_STATES[1];
    bool i3 = LED_STATES[2];
    bool i4 = LED_STATES[3];

    // Define some variables for the logic gates
    bool t1 = false;
    bool t2 = false;
    bool t3 = false;
    bool o = false;

    // Determine the state of t2
    //  - Is NOT i1 (thus, i1 inverted)
    //  - Is true if i1 is false, is false if i1 is true
    t1 = !i1;

    // Determine the state of t2
    //  - Is t1 AND i2
    //  - Is true if t1 AND i2 are both true
    t2 = t1 && i2;

    // Determine the state of t3
    //  - Is i3 OR i4
    //  - Is true if i3 OR i4 is true
    t3 = i3 || i4;

    // Determine the state of o
    //  - Is t2 OR t3, and then inverted
    //  - Is true if t2 OR t3 is true, and that result is also inverted
    o = !(t2 || t3);

    // Determine whether the output pin should be LOW or HIGH
    int outputState = LOW;
    if(o) {
        outputState = HIGH;
    }

    // Update the pin state of the output LED
    digitalWrite(LED_OUTPUT_PIN, outputState);
}