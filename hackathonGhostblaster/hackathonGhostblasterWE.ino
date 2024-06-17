#include <TM1637Display.h>
#include "IRremote.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
//#include "ghost.hpp"
#include <Servo.h>


// IR variables
int receiverPin = 11;
IRrecv irrecv(receiverPin);
decode_results results;

// Create instance of mp3 player
DFRobotDFPlayerMini DFPlayer;

#define CLK 9
#define DIO 8
#define timer 1000
#define alarm 7
TM1637Display display(CLK, DIO);
uint8_t data[] = {0, 0, 0, 0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long timertijd = 0;
int tm = 0;

#define seg11 22
#define seg12 23
#define seg13 25
#define seg14 24
#define seg15 26
#define seg16 27
#define seg17 28
#define seg21 30
#define seg22 31
#define seg23 32
#define seg24 33
#define seg25 34
#define seg26 36
#define seg27 35
int totalescore = 0;
int tiental = 0;
int eenheid = 0;
#define startKnop 52
short StartknopStatus = 0;
bool gameRunning = false;

short w = 0;
short x = 0;
short y = 0;
short z = 0;

unsigned long ghostCurrentMillis = 0;
unsigned long ghostPreviousMillis = 0;
#define lichtsen1 A10
#define lichtsen2 A11
#define lichtsen3 A12
#define lichtsen4 A13
#define lichtsen5 A14
#define lichtsen6 A15
int licht;
short pos1HIGH = 100;
short pos1LOW = 10;
short pos2HIGH = 100;
short pos2LOW = 10;
short pos3HIGH = 150;
short pos3LOW = 50;
short pos4HIGH = 0;
short pos4LOW = 80;
short pos5HIGH = 100;
short pos5LOW = 0;
short pos6HIGH = 0;
short pos6LOW = 100;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;



void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A1));
  pinMode(startKnop, INPUT);

  pinMode(seg11, OUTPUT); pinMode(seg12, OUTPUT); pinMode(seg13, OUTPUT); pinMode(seg14, OUTPUT); pinMode(seg15, OUTPUT); pinMode(seg16, OUTPUT); pinMode(seg17, OUTPUT);
  pinMode(seg21, OUTPUT); pinMode(seg22, OUTPUT); pinMode(seg23, OUTPUT); pinMode(seg24, OUTPUT); pinMode(seg25, OUTPUT); pinMode(seg26, OUTPUT); pinMode(seg27, OUTPUT);
  digitalWrite(seg21, HIGH); digitalWrite(seg22, HIGH); digitalWrite(seg23, HIGH); digitalWrite(seg24, HIGH); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, HIGH);
  digitalWrite(seg11, HIGH); digitalWrite(seg12, HIGH); digitalWrite(seg13, HIGH); digitalWrite(seg14, HIGH); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, HIGH);

  display.setBrightness(0x0f);
  data[3] = display.encodeDigit(0);
  data[2] = display.encodeDigit(0);
  data[1] = display.encodeDigit(0);
  data[0] = display.encodeDigit(0);
  display.setSegments(data);
  wachten();

  irrecv.enableIRIn();

  Serial2.begin(9600);

  if (!DFPlayer.begin(Serial2)) {
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  DFPlayer.volume(30); // Volume (0-30)
  DFPlayer.play(2); // play second mp3, to test (coin)

  servo1.attach(4);
  servo2.attach(2);
  servo3.attach(3);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(43);

  servo1.write(pos1HIGH);
  servo2.write(pos2HIGH);
  servo3.write(pos3HIGH);
  servo4.write(pos4HIGH);
  servo5.write(pos5HIGH);
  servo6.write(pos6HIGH);
  delay(500);
  servo1.write(pos1LOW);
  servo2.write(pos2LOW);
  servo3.write(pos3LOW);
  servo4.write(pos4LOW);
  servo5.write(pos5LOW);
  servo6.write(pos6LOW);
  
}

void loop() {
  StartknopStatus = digitalRead(startKnop);
  if((StartknopStatus == LOW) && (timertijd == 0)){
    digitalWrite(seg21, HIGH); digitalWrite(seg22, HIGH); digitalWrite(seg23, HIGH); digitalWrite(seg24, HIGH); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, HIGH);
    digitalWrite(seg11, HIGH); digitalWrite(seg12, HIGH); digitalWrite(seg13, HIGH); digitalWrite(seg14, HIGH); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, HIGH);
    display.setBrightness(0x0f);
    data[3] = display.encodeDigit(0);
    data[2] = display.encodeDigit(0);
    data[1] = display.encodeDigit(0);
    data[0] = display.encodeDigit(0);
    display.setSegments(data);
    wachten();
    gameRunning = 0;
    totalescore = 0;
    x = 0;
    StartknopStatus = digitalRead(startKnop);
  }else if((StartknopStatus == HIGH) or (x == 1)){
    if(x == 0){
      digitalWrite(seg21, HIGH); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
      digitalWrite(seg11, HIGH); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
      klokstart();
      timertijd = 101;
      totalescore = 0;
      x = 1;
    }
    x = 1;

    z = random(1,7);
    ghostPreviousMillis = millis();
    w = 0;
    
    while((millis() - ghostPreviousMillis < 1000) && (w == 0)){
    switch (z) {
      case 1:
        licht = analogRead(lichtsen1);  
        servo1.write(pos1HIGH); 
        klok();
        Serial.println(licht);
        if (licht > 500) {
          totalescore++;
          w++;
        }
        break;
      case 2:
        licht = analogRead(lichtsen2);  
        servo2.write(pos2HIGH); 
        klok();
        if (licht > 500) {
          totalescore++;
          w++;
        }
        break;
      case 3:
        licht = analogRead(lichtsen3);  
        servo3.write(pos3HIGH); 
        if (licht > 500) {
          totalescore++;
          w++;
        }
        break;
      case 4:
        licht = analogRead(lichtsen4);  
        servo4.write(pos4HIGH); 
        klok();
        if (licht > 500) {
          totalescore++;
          w++;
        }
        break;
      case 5:
        licht = analogRead(lichtsen5);  
        servo5.write(pos5HIGH); 
        klok();
        if (licht > 500) {
          totalescore++;
          w++;
        }
        break;
      case 6:
        licht = analogRead(lichtsen6);  
        servo6.write(pos6HIGH); 
        klok();
        if (licht > 500) {
          totalescore++;
          w++;
        }
        break;
    }
    ghostCurrentMillis = millis();
  }

  switch (z) {
    case 1:
      servo1.write(pos1LOW);
      break;
    case 2:
      servo2.write(pos2LOW);
      break;
    case 3:
      servo3.write(pos3LOW);
      break;
    case 4:
      servo4.write(pos4LOW);
      break;
    case 5:
      servo5.write(pos5LOW);
      break;
    case 6:
      servo6.write(pos6LOW);
      break;
  }

    
      score();
      klok();
      if (totalescore > 99){
        totalescore = 0;
      }
     if (timertijd == 0) {
      DFPlayer.play(2);
      delay(5000);
     }





    

  }
}

void klokstart(){
  y = 5;
  while (y != 0){
  display.setBrightness(0x0f);
  data[3] = display.encodeDigit(y);
  data[2] = display.encodeDigit(y);
  data[1] = display.encodeDigit(y);
  data[0] = display.encodeDigit(y);
  display.setSegments(data);
  y--;
  delay(1000);
  }
}
void klok () {
  currentMillis = millis();
  if ((currentMillis - previousMillis) >= 1000) {
    if (timertijd > 0) {
      timertijd--;
    }

    display.setBrightness(0x0f);
    data[3] = display.encodeDigit(timertijd / 1 % 10);
    data[2] = display.encodeDigit(timertijd % 60 / 10);
    data[1] = display.encodeDigit(timertijd / 60 % 10);
    data[0] = display.encodeDigit(timertijd / 60 / 10);
    display.setSegments(data);

    previousMillis = currentMillis;
  }
}

void score(){
  
  // Serial.print(totalescore);
  // Serial.print("       ");
  // Serial.print(tiental);
  // Serial.print(" ");
  // Serial.println(eenheid);
  
  eenheid = totalescore % 10;
  tiental = totalescore / 10;
  
  if(eenheid == 0){
    digitalWrite(seg21, HIGH); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }else if(eenheid == 1){
    digitalWrite(seg21, HIGH); digitalWrite(seg22, HIGH); digitalWrite(seg23, HIGH); digitalWrite(seg24, LOW); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, LOW);
  }else if(eenheid == 2){
    digitalWrite(seg21, LOW); digitalWrite(seg22, HIGH); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, HIGH);
  }else if(eenheid == 3){
    digitalWrite(seg21, LOW); digitalWrite(seg22, HIGH); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, HIGH); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }else if(eenheid == 4){
    digitalWrite(seg21, LOW); digitalWrite(seg22, LOW); digitalWrite(seg23, HIGH); digitalWrite(seg24, LOW); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, LOW);
  }else if(eenheid == 5){
    digitalWrite(seg21, LOW); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, HIGH); digitalWrite(seg25, HIGH); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }else if(eenheid == 6){
    digitalWrite(seg21, LOW); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, HIGH); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }else if(eenheid == 7){
    digitalWrite(seg21, HIGH); digitalWrite(seg22, HIGH); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, LOW);
  }else if(eenheid == 8){
    digitalWrite(seg21, LOW); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }else if(eenheid == 9){
    digitalWrite(seg21, LOW); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, HIGH); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }else{
    digitalWrite(seg21, HIGH); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
  }

  
  if(tiental == 0){
    digitalWrite(seg11, HIGH); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  }else if(tiental == 1){
    digitalWrite(seg11, HIGH); digitalWrite(seg12, HIGH); digitalWrite(seg13, HIGH); digitalWrite(seg14, LOW); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, LOW);
  }else if(tiental == 2){
    digitalWrite(seg11, LOW); digitalWrite(seg12, HIGH); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, HIGH);
  }else if(tiental == 3){
    digitalWrite(seg11, LOW); digitalWrite(seg12, HIGH); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, HIGH); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  }else if(tiental == 4){
    digitalWrite(seg11, LOW); digitalWrite(seg12, LOW); digitalWrite(seg13, HIGH); digitalWrite(seg14, LOW); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, LOW);
  }else if(tiental == 5){
    digitalWrite(seg11, LOW); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, HIGH); digitalWrite(seg15, HIGH); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  }else if(tiental == 6){
    digitalWrite(seg11, LOW); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, HIGH); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  }else if(tiental == 7){
    digitalWrite(seg11, HIGH); digitalWrite(seg12, HIGH); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, LOW);
  }else if(tiental == 8){
    digitalWrite(seg11, LOW); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  }else if(tiental == 9){
    digitalWrite(seg11, LOW); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, HIGH); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  }else{
    digitalWrite(seg11, HIGH); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
  } 
}

void wachten(){
 int dealay = 120;
 digitalWrite(seg23, HIGH); digitalWrite(seg24, HIGH);
 digitalWrite(seg23, LOW); digitalWrite(seg13, LOW);
 delay(dealay);
 digitalWrite(seg23, HIGH); digitalWrite(seg13, HIGH);
 digitalWrite(seg12, LOW); digitalWrite(seg13, LOW);
 delay(dealay);
 digitalWrite(seg12, HIGH); digitalWrite(seg13, HIGH);
 digitalWrite(seg15, LOW); digitalWrite(seg12, LOW);
 delay(dealay);
 digitalWrite(seg15, HIGH); digitalWrite(seg12, HIGH);
 digitalWrite(seg16, LOW); digitalWrite(seg15, LOW);
 delay(dealay);
 digitalWrite(seg16, HIGH); digitalWrite(seg15, HIGH);
 digitalWrite(seg26, LOW); digitalWrite(seg16, LOW);
 delay(dealay);
 digitalWrite(seg26, HIGH); digitalWrite(seg16, HIGH);
 digitalWrite(seg27, LOW); digitalWrite(seg26, LOW);
 delay(dealay);
 digitalWrite(seg27, HIGH); digitalWrite(seg26, HIGH);
 digitalWrite(seg24, LOW); digitalWrite(seg27, LOW);
 delay(dealay);
 digitalWrite(seg24, HIGH); digitalWrite(seg27, HIGH);
 digitalWrite(seg23, LOW); digitalWrite(seg24, LOW);
 delay(dealay);
}
