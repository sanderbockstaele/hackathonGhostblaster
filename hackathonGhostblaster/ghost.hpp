#include "Arduino.h"
#include "HardwareSerial.h"
#include <Servo.h>

#define baseLightLevel 300
#define up 100
#define down 0

#define choiceInterval 1000

bool hit(uint8_t pin, DFRobotDFPlayerMini DFPlayer);
void ghostDown(uint8_t ghostPin);
void setGhostState(int ghost);

Servo servo;

unsigned long ghostCurrentMillis = 0;
unsigned long ghostPreviousMillis = 0;

unsigned long currentShotMilis = 0;
unsigned long previousShotMilis = 0;

int ghostPin[] = {PB4, PB2, PB3, PB5, PB6, 10};
int ghostVisible[] = {0, 100, 150, 0, 180, 100};
int ghostInvisible[] = {100, 10, 50, 80, 100, 0};
bool ghostStatus[] = {false, false, false, false, false, false}; //true = up, false = down

int ghostShown = 0;

bool hit(uint8_t pin, DFRobotDFPlayerMini DFPlayer){
  int ghostId = ghostPin[pin-65];
  if (analogRead(pin) > baseLightLevel){
    currentShotMilis = millis();
    if (currentShotMilis - previousShotMilis > 500){
      Serial.print(ghostId); Serial.println("BIEM");
      DFPlayer.play(2); // Play second mp3 (hit)
      ghostDown(ghostId);
      previousShotMilis = currentShotMilis;
      return true;
    }
  }
  return false;
}

void ghostDown(uint8_t ghostId){
  Serial.println("ENTER DOWN");

  servo.attach(ghostPin[ghostId]);
  servo.write(ghostInvisible[ghostId]);
  ghostStatus[ghostId] = false;
}

void ghostUp(uint8_t ghostId){
  servo.attach(ghostPin[ghostId]);
  servo.write(ghostVisible[ghostId]);
  ghostStatus[ghostId] = true;
}