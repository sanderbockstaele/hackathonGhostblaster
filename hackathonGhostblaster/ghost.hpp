#include "Arduino.h"
#include "HardwareSerial.h"
#include <Servo.h>

#define baseLightLevel 180
#define up 100
#define down 0

#define choiceInterval 1000

bool hit(uint8_t pin);
void ghostDown(uint8_t ghostPin);
void setGhostState(int ghost);

Servo servo;

unsigned long ghostCurrentMillis = 0;
unsigned long ghostPreviousMillis = 0;

unsigned long currentShotMilis = 0;
unsigned long previousShotMilis = 0;

int ghostPin[] = {PB4, PB2, PB3, PB5, PB6};
int ghostVisible[] = {0, 100, 150, 0, 100};
int ghostInvisible[] = {100, 10, 50, 80, 0};
bool ghostStatus[] = {false, false, false, false, false}; //true = up, false = down

int ghostShown = 0;

bool hit(uint8_t pin){
  int ghostId = ghostPin[pin-64];
  Serial.println(analogRead(pin));
  if (analogRead(pin) > baseLightLevel){
    currentShotMilis = millis();
    if (currentShotMilis - previousShotMilis > 100){
      Serial.println("BIEM");
      ghostDown(ghostId);
      return true;
    }
  }
  return false;
}

void ghostDown(uint8_t ghostId){
  servo.attach(ghostPin[ghostId]);
  servo.write(ghostInvisible[ghostId]);
  ghostStatus[ghostId] = false;
}

void ghostUp(uint8_t ghostId){
  servo.attach(ghostPin[ghostId]);
  servo.write(ghostVisible[ghostId]);
  ghostStatus[ghostId] = true;
}