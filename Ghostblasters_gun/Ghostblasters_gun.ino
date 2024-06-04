
/* =========== CONNECTING ESP8266 ===========
driver for connecting on windows: https://www.silabs.com/documents/public/software/CP210x_Windows_Drivers.zip
IDE: file menu > preference > Additional Board Manager: http://arduino.esp8266.com/stable/package_esp8266com_index.json
IDE: board manager > download ESP8266
IDE: tools > board manager > ESP8266 > NodeMCU 1.0 (ESP-12E Module)

=== connecting IR LED ===
gebaseerd op de IRsendDemo van de IRremoteesp8266 library
schema komt van: https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
*/


#include <U8g2lib.h> // WifiKit8 display

#include <IRremoteESP8266.h> // IR on ESP8266
#include <IRsend.h> // sending IR signals



#define trigger D7
#define laser D8

const uint16_t kIrLed = D2;
IRsend irsend(kIrLed);

// U8g2 Contructor for WiFi Kit 8
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

// refresh screen
unsigned long previousRefreshMillis = 0;
const long screenRefreshInterval = 1000;

// bullets in gun
const int MAX_BULLETS = 10;
int bullets = MAX_BULLETS;

// trigger and reload variables
int triggerState;

int lastTriggerState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 

const int reloadTime = 2000;

// laser variable
int laserState = LOW;

void setup() {
  u8g2.begin();
  irsend.begin();

  pinMode(trigger, INPUT_PULLUP);
  pinMode(laser, OUTPUT);

  digitalWrite(laser, laserState); // make sure laser is off

  updateScreen(); // start display
}

void loop() {
  unsigned long currentRefreshMillis = millis();

  int reading = digitalRead(trigger);

  if (reading != lastTriggerState) {
    // reser de timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != triggerState) {
      triggerState = reading;

      // if trigger is low, shoot (Normal Closed trigger)
      if (triggerState == LOW) {
        laserState = !laserState;
        if (laserState == HIGH) {
          if (bullets <= 0) {
            reload();
          } else {
            bullets--;

            // Send shoot signal with IR
            uint32_t irCode = 0x00FFA25D;
            irsend.sendNEC(irCode, 32);
          }
          updateScreen();
        }
      }
    }
  }

  // update laserState and reading
  digitalWrite(laser, laserState);
  lastTriggerState = reading;

  // stop laser if trigger is not pressed
  if (triggerState == HIGH && millis() - lastDebounceTime > debounceDelay) {
    laserState = LOW;
    digitalWrite(laser, laserState);
  }
}

// function for updateing the display
void updateScreen() {
  u8g2.setFont(u8g2_font_logisoso32_tf);
  u8g2.firstPage();
  do {
    u8g2.drawUTF8(0, 32, (String(bullets) + "/" + String(MAX_BULLETS)).c_str());
  } while ( u8g2.nextPage() );
}

// Display reloading and set bullets to max
void reload() {
  u8g2.setFont(u8g2_font_logisoso22_tf);
  u8g2.firstPage();
  do {
    u8g2.drawUTF8(0, 32, "RELOADING...");
  } while ( u8g2.nextPage() );

  delay(reloadTime);
  bullets = MAX_BULLETS;
}