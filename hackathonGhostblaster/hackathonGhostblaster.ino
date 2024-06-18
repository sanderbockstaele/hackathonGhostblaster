#include <TM1637Display.h>
#include "IRremote.h"
#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "ghost.hpp"

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
#define startKnop 53
#define ledPin 52
short StartknopStatus = 0;

void setup() {
  Serial.begin(9600);
  pinMode(startKnop, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

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
  DFPlayer.play(2); // play second mp3, to test (coin)ghostUp(0); 


}

void loop() {
  // Prepare score display
  digitalWrite(seg21, HIGH); digitalWrite(seg22, HIGH); digitalWrite(seg23, HIGH); digitalWrite(seg24, HIGH); digitalWrite(seg25, HIGH); digitalWrite(seg26, HIGH); digitalWrite(seg27, HIGH);
  digitalWrite(seg11, HIGH); digitalWrite(seg12, HIGH); digitalWrite(seg13, HIGH); digitalWrite(seg14, HIGH); digitalWrite(seg15, HIGH); digitalWrite(seg16, HIGH); digitalWrite(seg17, HIGH);
  // Prepare time display
  display.setBrightness(0x0f);
  data[3] = display.encodeDigit(0);
  data[2] = display.encodeDigit(0);
  data[1] = display.encodeDigit(0);
  data[0] = display.encodeDigit(0);
  display.setSegments(data);

  wachten(); // Display a waiting pattern
  totalescore = 0; // Zero the score


  ghostDown(0);
  ghostDown(1);
  ghostUp(2);
  ghostDown(3);
  ghostDown(4);

  StartknopStatus = digitalRead(startKnop);
  // If the startbutton is pressed
  if(StartknopStatus == LOW){
    digitalWrite(ledPin, HIGH);
    // Clear score display
    digitalWrite(seg21, HIGH); digitalWrite(seg22, LOW); digitalWrite(seg23, LOW); digitalWrite(seg24, LOW); digitalWrite(seg25, LOW); digitalWrite(seg26, LOW); digitalWrite(seg27, LOW);
    digitalWrite(seg11, HIGH); digitalWrite(seg12, LOW); digitalWrite(seg13, LOW); digitalWrite(seg14, LOW); digitalWrite(seg15, LOW); digitalWrite(seg16, LOW); digitalWrite(seg17, LOW);
    
    klokstart(); // Do countdown

    timertijd = 101; // Set time 

    // While the game is going on
    while (timertijd > 0) {
      // CHeck for IR from gun
      if (irrecv.decode(&results)) { // recieved signal?
        DFPlayer.play(1); // Play first mp3 (shot)
        irrecv.resume(); // Wait for next
      }

      // Loop over all possible ldrs
      for (int i = A10; i < A15; i++){
        if (hit(i, DFPlayer))
          totalescore++; // Increase score if hit
      }

      // Every second change the state  from up to down or vice versa
      unsigned long ghostCurrentMillis = millis();

      if (ghostCurrentMillis - ghostPreviousMillis >= choiceInterval) {
        ghostPreviousMillis = ghostCurrentMillis;

        int ghostId = random(0,4);

        if(ghostStatus[ghostId] == false){
          ghostUp(ghostId);
        }
        else{
          ghostDown(ghostId);
        }
      }

      score(); // Display score
      klok(); // Advance and display clock
      if (totalescore > 99) // handle score overflow
        totalescore = 0;
    }
    DFPlayer.play(2);
    delay(5000);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}

void klokstart(){
  int countDown = 5;
  while (countDown != 0){
  display.setBrightness(0x0f);
  data[3] = display.encodeDigit(countDown);
  data[2] = display.encodeDigit(countDown);
  data[1] = display.encodeDigit(countDown);
  data[0] = display.encodeDigit(countDown);
  display.setSegments(data);
  countDown--;
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