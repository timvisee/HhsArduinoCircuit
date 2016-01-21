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
 * Wordt aangeroepen als de Arduino start.
 */
void setup() {
    // Stel de input LED pinnen in
    for(int i = 0; i < LED_INPUT_COUNT; i++) {
        pinMode(LED_INPUT_PIN[i], OUTPUT);
    }

    // Stel de output LED pinnen in
    pinMode(LED_OUTPUT_PIN, OUTPUT);

    // Stel de twee buttons in
    pinMode(BUTTON_SHIFT_PIN, INPUT);
    pinMode(BUTTON_TOGGLE_PIN, INPUT);
}

/**
 * Wordt aangeroepen elke keer aan geroepen na de setup() functie.
 */
void loop() {
    // Definieer twee variabelen voor de verschuif en omzet knoppen
    bool shiftButton = false;
    bool toggleButton = false;

    // Kijk of de verschuif knop is ingedrukt, sla het resultaat op in de variabele
    if(digitalRead(BUTTON_SHIFT_PIN) == HIGH) {
        shiftButton = true;
    }

    // Kijk of de omzet knop is ingedrukt, sla het resultaat op in de variabele
    if(digitalRead(BUTTON_TOGGLE_PIN) == HIGH) {
        toggleButton = true;
    }

    // Kijk of op dit moment de verschuif knop ingedrukt of losgelaten wordt
    if(shiftButton != shiftButtonState) {
        // Als de knop wordt ingedrukt
        if(shiftButton){
            // Verschuif de geselecteerde LED (of zet de edit modus aan)
            shiftSelectedLed();
        }

        // Wacht even om problemen met de button te voorkomen
        delay(BUTTON_JITTER_DELAY);
    }

    // Kijk of op dit moment de omzet knop ingedrukt of losgelaten wordt
    if(toggleButton != toggleButtonState) {
        // Als de knop wordt ingedrukt
        if(toggleButton) {
            // Zet de geselecteerde LED om (alleen als de edit modus aan staat)
            toggleSelectedLed();
        }

        // Wacht even om problemen met de button te voorkomen
        delay(BUTTON_JITTER_DELAY);
    }

    // Sla de statussen van de knoppen op in de variabelen
    shiftButtonState = shiftButton;
    toggleButtonState = toggleButton;

    // Bereken het resultaat van het circuit, en toon het resultaat bij het output lampje
    doLogic();

    // Stel de status van de input LEDs in gebaseerd op de invoer
    updateLeds();
}

/**
 * Verschuif de geselecteerde LED met 1 positie, of zet de edit modus aan als deze nog niet aan staat.
 */
void shiftSelectedLed() {
    // Zet de edit modus aan als deze nog niet aan staat
    if(editMode == false) {
        // Zet de modus aan
        editMode = true;

        // Selecteer de eerste LED
        selectedLed = 0;

        // Ga de functie uit, het selecteren van de volgende LED moet pas de volgende keer
        return;
    }

    // Selecteer de volgende LED
    selectedLed++;

    // Zet de edit modus weer uit als de laatste LED geselecteerd was
    if(selectedLed >= LED_INPUT_COUNT) {
        editMode = false;
    }
}

/**
 * Zet de geselecteerde LED om.
 */
void toggleSelectedLed() {
    // Draai de status om van de geselecteerde LED
    ledStates[selectedLed] = !ledStates[selectedLed];

    // Determine whether the new state is on or off
    int newLedState = LED_BRIGHTNESS_OFF;
    if(ledStates[selectedLed] == true) {
        newLedState = LED_BRIGHTNESS_ON;
    }

    // Bereken de circuit logica opnieuw
    doLogic();

    // Stel de status van alle LEDs opnieuw in
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
        if(ledStates[i] == true) {
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
       || (i1 && i2 && !i3 && !i4) == true) {
        digitalWrite(LED_OUTPUT_PIN, HIGH);
    } else {
        digitalWrite(LED_OUTPUT_PIN, LOW);
    }
}