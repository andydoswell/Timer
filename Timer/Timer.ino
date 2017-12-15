/* Low power countdown timer.
   Copyright A.G.Doswell May 2017
   License: The MIT License (See full license at the bottom of this file)
   Visit Andydoz.blogspot.com for description and circuit.

*/


#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

const int beeperPin = 3; //defines pin the beeper is connected to.
const int buttonPin = 2; // defines where the N/O button is connected
const int potPin = A0; // analogue input from pot
const int potPower = 13; // used to switch power on to the pot
const int displayPwrPin = A3; // defines the pin used to connect power to the display
boolean timerFlag; // false when timer has expired
int seconds; // no of seconds
const byte PIN_CLK = 11;   // define CLK pin for LED "lightube"
const byte PIN_DIO = 10;   // define DIO pin for LED "lightube"
SevenSegmentExtended      display(PIN_CLK, PIN_DIO);
long oldMillis;

void setup()
{
  pinMode (beeperPin, OUTPUT);
  pinMode (displayPwrPin, OUTPUT);
  digitalWrite(displayPwrPin,HIGH); // switch power on to the display
  digitalWrite (beeperPin, LOW);
  pinMode (buttonPin, INPUT_PULLUP);
  pinMode (potPower, OUTPUT);
  digitalWrite (potPower, LOW);
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  delay(1000);
  setTimerVal ();
}

void loop()
{
  if (!timerFlag) {
    sleep ();
  }
  for (; seconds >= 0; seconds--) {
    updateDisplay ();
    while (millis() <= (oldMillis + 1000));
    oldMillis = millis();
  }
  timerFlag = false;
}

void setTimerVal () {
  digitalWrite (potPower, HIGH); // switch power to pot.
  for (long x = 0; x < 100; x++) {
    int timerMinutes = analogRead(potPin);
    timerMinutes = map( timerMinutes, 0, 1000 , 1 , 45);
    seconds = timerMinutes * 60;
    updateDisplay();
  }
  timerFlag = true;
  digitalWrite (potPower, LOW); //done with the pot, so switch it off
  oldMillis = millis();
}

void sleep() {
  attachInterrupt(0, buttonPin, LOW);   // Set pin 2 as interrupt and attach handler
  for (int x = 0; x < 6; x++) { // bleep 6 times
    digitalWrite (beeperPin, HIGH);
    delay (250);
    digitalWrite (beeperPin, LOW);
    display.setBacklight(0);
    delay (250);
    display.setBacklight(100);
  }
  delay (5000);
  digitalWrite (displayPwrPin, LOW); // switch off power to display
  delay (100);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //set sleep mode
  sleep_enable(); // enable sleep
  sleep_mode(); // and ZzZzZzZz....    //
  sleep_disable();
  digitalWrite (displayPwrPin, HIGH); // switch on power to display
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  setTimerVal ();
}

void updateDisplay () { // update the LED display
  byte minutes = seconds / 60;
  byte displaySeconds = seconds % 60;
  display.printTime(minutes, displaySeconds, true);
}


/*
   Copyright (c) 2017 Andrew Doswell

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute and sublicense
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   Any commercial use is prohibited without prior arrangement.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
