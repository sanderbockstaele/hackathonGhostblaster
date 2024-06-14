/*  =========== CONNECTING DFrobot mp3 mini ===========
code for DFrobot based of https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#target_6
Connect rx of mp3 player to Serial2 tx (16) of Arduino mega with 1k ohm resistor
Connect tx of mp3 player to Serial2 rx (17) of Arduino mega
*/


#include "IRremote.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// IR variables
int receiver = 11;
IRrecv irrecv(receiver);
decode_results results;

// Create instance of mp3 player
DFRobotDFPlayerMini DFPlayer;

void setup() {
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn(); // Start IRRecv

  // Use Serial2, Serial1 didn't work on Arduino Mega
  Serial2.begin(9600);

  // Try connection
  if (!DFPlayer.begin(Serial2)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  DFPlayer.volume(30); // Volume (0-30)
  DFPlayer.play(2); // play second mp3, to test (coin)
}

void loop() {
  if (irrecv.decode(&results)) { // recieved signal?
    Serial.println(results.value, HEX);
    if (results.value == 0xDECAFE) { // IR signal of gun is DECAFE
      DFPlayer.play(1); // Play first mp3 (shot)
    }
    delay(500);
    irrecv.resume(); // Wait for next
  }
}
