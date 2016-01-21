/**
 * "circuit" opdracht.
 *
 * Met dit programma voor de Arduino kun je een circuit simuleren. Er zijn vier lampjes, deze kun je door middel van
 * twee knoppen aan of uit zetten. Het vijfde 'output' lampje laat het gesimuleerde resultaat zien van het circuit.
 *
 * Gemaakt door Ming.
 */

#include <Arduino.h>

/**
 * Aantal LEDs als input.
 */
const int LED_INPUT_COUNT = 4;

/**
 * Arduino pinnen van de input LEDs.
 */
int LED_INPUT_PIN[LED_INPUT_COUNT] = {5, 6, 9, 10};

/**
 * Arduino pin van de output LED.
 */
int LED_OUTPUT_PIN = 12;

/**
 * Arduino pin van de knop die gebruikt kan worden om de volgende LED te selecteren, of om de edit modus aan te in
 * te gaan.
 */
int BUTTON_SHIFT_PIN = 7;

/**
 * Arduino pin van de knop die gebruikt kan worden om de geselecteerde LED om te zetten in de edit modus.
 */
int BUTTON_TOGGLE_PIN = 8;

/**
 * Het aantal millisecondes dat de Arduino moet wachten als een knop ingedrukt of losgelaten wordt. Om problemen met
 * 'bouncing' of 'jitter' te voorkomen.
 */
int BUTTON_JITTER_DELAY = 50;

/**
 * Defines the number of milliseconds to show the new state of a LED before making it dimmed again.
 */
int LED_TOGGLE_SHOW_DURATION = 1000;

/**
 * De felheid van de LEDs als ze uit moeten staan, van 0 tot 255.
 */
int LED_BRIGHTNESS_OFF = 0;

/**
 * De felheid van de LEDs als ze vaag moeten branden, van 0 tot 255.
 */
int LED_BRIGHTNESS_ON = 255;

/**
 * De felheid van de LEDs als ze aan moeten staan, van 0 tot 255.
 */
int LED_BRIGHTNESS_DIM = 30;

/**
 * Een array waar de status van elke LED in wordt opgeslagen. True betekent dat de LED aan staa, false betekent uit.
 * Deze array bevat een status voor elk van de input LEDs.
 */
bool ledStates[LED_INPUT_COUNT] = {false, false, false, false};

/**
 * Deze variabele slaat de laatste status van de verschuif knop op. True betekent dat de knop de laatste keer was
 * ingedrukt, false betekent dat de knop niet is ingedrukt.
 */
bool shiftButtonState = false;

/**
 * Deze variabele slaat de laatste status van de omzet knop op. True betekent dat de knop de laatste keer was
 * ingedrukt, false betekent dat de knop niet is ingedrukt.
 */
bool toggleButtonState = false;

/**
 * De geselecteerde LED. 0 is de eerste LED, 3 de laatste.
 */
int selectedLed = 0;

/**
 * Specificeert of de edit modus aan staat. True betekent dat de modus aan staat, false dat de modus uit staat.
 */
bool editMode = false;



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
        // Shift the selected LED, but only if the button is pressed down now
        if(shiftButton){
            shiftSelectedLed();
        }

        // Wait for a little while to prevent button jitter (/bouncing)
        delay(BUTTON_JITTER_DELAY);
    }

    // Check whether the state of the toggle button has changed
    if(toggleButton != toggleButtonState) {
        // Toggle the selected LED, but only if the button is pressed down now
        if(toggleButton) {
            toggleSelectedLed();
        }

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
    // Start the edit mode if the device isn't in edit mode yet
    if(!editMode) {
        // Start the edit mode
        editMode = true;

        // Reset the selected LED
        selectedLed = 0;

        // Leave the function to handle everything in the edit mode
        return;
    }

    // Increase the index
    selectedLed++;

    // If the new index is out of bound, disable the edit mode
    if(selectedLed >= LED_INPUT_COUNT) {
        editMode = false;
    }
}

/**
 * Toggle the selected LED.
 */
void toggleSelectedLed() {
    // Toggle the state of the selected LED
    ledStates[selectedLed] = !ledStates[selectedLed];

    // Determine whether the new state is on or off
    int newLedState = LED_BRIGHTNESS_OFF;
    if(ledStates[selectedLed]) {
        newLedState = LED_BRIGHTNESS_ON;
    }

    // Do the program logic
    doLogic();

    // Update the LED states
    updateLeds();
}

/**
 * Update the visual state of each LED.
 */
void updateLeds() {
    // Loop through all input LEDs and update their visual state
    for(int i = 0; i < LED_INPUT_COUNT; i++) {
        // Determine whether the new state should be on or off
        int newState = LED_BRIGHTNESS_OFF;
        if(ledStates[i]) {
            newState = LED_BRIGHTNESS_ON;
        }

        // If the edit mode is turned on, and this LED isn't selected, dim the LED
        if(editMode && selectedLed != i) {
            newState = LED_BRIGHTNESS_DIM;
        }

        // Write the state to the LED pin
        analogWrite(LED_INPUT_PIN[i], newState);
    }
}

/**
 * Do the "circuit" logic with the current button states.
 */
void doLogic() {
    // Define some variables with their states for the inputs
    bool i1 = ledStates[0];
    bool i2 = ledStates[1];
    bool i3 = ledStates[2];
    bool i4 = ledStates[3];

//    // Do the circuit logic, and write the result to the output pin
//    if(!((!i1 && i2) || (i3 || i4))) {
//        digitalWrite(LED_OUTPUT_PIN, HIGH);
//    } else {
//        digitalWrite(LED_OUTPUT_PIN, LOW);
//    }

    // Do the circuit logic, and write the result to the output pin
    if((!i1 && !i2 && !i3 && !i4)
       || (i1 && !i2 && !i3 && !i4)
       || (i1 && i2 && !i3 && !i4)) {
        digitalWrite(LED_OUTPUT_PIN, HIGH);
    } else {
        digitalWrite(LED_OUTPUT_PIN, LOW);
    }
}