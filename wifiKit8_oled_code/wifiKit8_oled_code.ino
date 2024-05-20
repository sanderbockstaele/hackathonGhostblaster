
/* =========== CONNECTING ESP8266 ===========
driver for connecting on windows: https://www.silabs.com/documents/public/software/CP210x_Windows_Drivers.zip
IDE: file menu > preference > Additional Board Manager: http://arduino.esp8266.com/stable/package_esp8266com_index.json
IDE: board manager > download ESP8266
IDE: tools > board manager > ESP8266 > NodeMCU 1.0 (ESP-12E Module)
*/

#include <U8g2lib.h>

// U8g2 Contructor for WiFi Kit 8
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

// refresh screen
unsigned long previousMillis = 0;
const long screenRefrechInterval = 1000;

// bullets in gun
const int MAX_BULLETS = 10;
int bullets = 8;

void setup() {
  u8g2.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= screenRefrechInterval) {
    previousMillis = currentMillis; // update timer

    u8g2.setFont(u8g2_font_logisoso32_tf); // font and font-size
    u8g2.firstPage(); // render new display
    do {
      u8g2.drawUTF8(0, 32, (String(bullets) + "/" + String(MAX_BULLETS)).c_str()); // x,y starting point and display text as const char*
    } while ( u8g2.nextPage() );
  }
}