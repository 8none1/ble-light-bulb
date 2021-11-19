#include <Arduino.h>
#include <BLEDevice.h>
#include "BLELight.h"

BLELight bleLight;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");
  }


void loop() {
  if(bleLight.connect(BLEAddress("78:82:a4:00:05:1e"))) { // Note: Sometimes need to connect multiple times 
    bleLight.turnOn();
    delay(1000);
    bleLight.turnOff();
    delay(1000);
    bleLight.turnOn();
    delay(1000);
    bleLight.setColor(0xFF, 0x00, 0x00);//, 100, false);
    delay(1000); 

    bleLight.setColor(0x00, 0xFF, 0x00);//, 100, false);
    delay(1000);

    bleLight.setColor(0x00, 0x00, 0xFF);// , 100, false);
    delay(1000);

    bleLight.setMode(RGB_STROBE, 30);
    delay(10000);
    bleLight.turnOff();

    bleLight.getStatus();
    Serial.println("Sent status request");
    delay(5000);
    Serial.println("Done");
    bleLight.disconnect();
  }
  Serial.println("Send me a key press to continue");
  while (Serial.available() == 0) {
    delay(100);
  }
  while (Serial.available()) char a = Serial.read();
}
