#include <TM1637Display.h>
#include "IRremote.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "ghost.hpp"

// IR variables
int receiver = A8;
IRrecv irrecv(receiver);
decode_results results;

// Create instance of mp3 player
DFRobotDFPlayerMini DFPlayer;

#define CLK 9
#define DIO 8
#define timer 1000
#define alarm 7
#define buzztijd 500
TM1637Display display(CLK, DIO);
uint8_t data[] = {0, 0, 0, 0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long timertijd = 0;
int tm = 0;

#define seg11 22
#define seg12 23
#define seg13 24
#define seg14 25
#define seg15 26
#define seg16 27
#define seg17 28
#define seg21 30
#define seg22 31
#define seg23 32
#define seg24 33
#define seg25 34
#define seg26 35
#define seg27 36
int totalescore = 0;
int tiental = 0;
int eenheid = 0;
#define startKnop 20
short StartknopStatus = 0;
short x = 0;
short y = 0;

void setup() {
  Serial.begin(9600);
  pinMode(alarm, OUTPUT);
  noTone(alarm);
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
  // wachten();

  ghostUp(1);
  delay(10000);
  ghostDown(1);
}

void loop() {
  StartknopStatus = digitalRead(startKnop);
  if((StartknopStatus == LOW) && (timertijd == 0)){
    Serial.println(1);
    digitalWrite(seg21, HIGH); digitalWrite(seg22, HIGH); digitalWrite(seg23, HIGH); digitalWrite(seg24, HIGH); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, HIGH);
    digitalWrite(seg11, HIGH); digitalWrite(seg12, HIGH); digitalWrite(seg13, HIGH); digitalWrite(seg14, HIGH); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, HIGH);
    display.setBrightness(0x0f);
    data[3] = display.encodeDigit(0);
    data[2] = display.encodeDigit(0);
    data[1] = display.encodeDigit(0);
    data[0] = display.encodeDigit(0);
    display.setSegments(data);
    wachten();
    x = 0;
    totalescore = 0;
    StartknopStatus = digitalRead(startKnop);
    Serial.println("LOL");
    Serial.println(StartknopStatus);
  }else if((StartknopStatus == HIGH) or (x == 1)){
    Serial.println(2);
    if(x == 0){
      digitalWrite(seg21, HIGH); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
      digitalWrite(seg11, HIGH); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
      klokstart();
      timertijd = 101;
      totalescore = 0;


      ghostDown(0);
      ghostDown(1);
      ghostDown(2);
      ghostDown(3);
      ghostDown(4);
      ghostDown(5);
    }

    if (irrecv.decode(&results)) { // recieved signal?
      Serial.println(results.value, HEX);
        if (results.value == 0xDECAFE) { // IR signal of gun is DECAFE
          DFPlayer.play(1); // Play first mp3 (shot)
        }
      delay(500);
      irrecv.resume(); // Wait for next
    }

    for (int i = A10; i < A15; i++){
      if (hit(i))
        totalescore++;
    }

    unsigned long ghostCurrentMillis = millis();

    if (ghostCurrentMillis - ghostPreviousMillis >= choiceInterval) {
      ghostPreviousMillis = ghostCurrentMillis;

      int ghostId = random(0,6);

      if(ghostStatus[ghostId] == false){
        ghostUp(ghostId);
      }
      else{
        ghostDown(ghostId);
      }
    }

    x = 1;
    score();
    klok();
    if (timertijd == 0) {
    while (tm < 3){ 
      tone(alarm, 500);
      delay(buzztijd);
      noTone(alarm);
      delay(buzztijd);
      tm++;
    }
    delay(10000);
  }
  if (totalescore > 100) {
    totalescore = 0;
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
  
  Serial.print(totalescore);
  Serial.print("       ");
  Serial.print(tiental);
  Serial.print(" ");
  Serial.println(eenheid);
  
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
