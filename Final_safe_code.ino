#include <Keypad.h>
#include <FastLED.h>

#include <Servo.h>



#define LED_PIN     13
#define NUM_LEDS    23
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define UPDATES_PER_SECOND 100

// Create two Servo objects to control the motors
Servo servo1;
Servo servo2;

CRGB leds[NUM_LEDS];
uint8_t palette_Index = 0;

const byte ROWS = 4; 
const byte COLS = 4; 

const int passwordLength = 6;  // 1 more character than the length of the correct password
char password_variable[passwordLength] = "";  // Stores entered password
char correct_code[] = "165AB";  // Correct password
bool door_open = false;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);

  // LED setup
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Set the default color to yellow for all LEDs
  fill_solid(leds, NUM_LEDS, CRGB::Yellow);
  FastLED.show();
  servo1.attach(11);  // Attach the first servo to pin 11
  servo2.attach(12); // Attach the second servo to pin 12
}

void loop() {
  char customKey = customKeypad.getKey();
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  if (customKey) {
    Serial.println(customKey);

    if (customKey == '#') {
      // Reset password
      memset(password_variable, 0, passwordLength);  // Clear the array
      Serial.println("Password reset");

      // Set all LEDs to yellow
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      FastLED.show();

      if (door_open == true) {
        //close door
        servo1.write(180);  // Servo 1 to 180 degrees
        servo2.write(0);  // Servo 2 to 0 degrees (opposite direction)
        delay(1000); // Wait for 1 second
      }

    } else if (customKey == '*') {
      // Verify password
      if (strcmp(password_variable, correct_code) == 0) {
        Serial.println("Correct. Opening now");
          fill_solid(leds, NUM_LEDS, CRGB::Green);
          FastLED.show();
          if (door_open == false) {
          //close door
          servo1.write(0);  // Servo 1 to 180 degrees
          servo2.write(180);  // Servo 2 to 0 degrees (opposite direction)
          door_open = true;
          delay(1000); // Wait for 1 second
          
        }

        // Set all LEDs to green
        
      } else {
        Serial.println("Incorrect");

        // Set all LEDs to red
        fill_solid(leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
      }
      memset(password_variable, 0, passwordLength);  // Clear for new input
    } else {
      // Append character to password (if not full)
      int len = strlen(password_variable);
      if (len < passwordLength - 1) {
        password_variable[len] = customKey;
        password_variable[len + 1] = '\0';  // Ensure null termination
      }

      // Set all LEDs to yellow
      fill_solid(leds, NUM_LEDS, CRGB::Yellow);
      FastLED.show();

      if (door_open == true) {
        //close door
        servo1.write(180);  // Servo 1 to 180 degrees
        servo2.write(0);  // Servo 2 to 0 degrees (opposite direction)
        delay(1000); // Wait for 1 second
      }

    }
  }

  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
