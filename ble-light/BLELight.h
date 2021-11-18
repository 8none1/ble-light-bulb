#ifndef _BLE_LIGHT_H_
#define _BLE_LIGHT_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Works with Consmart/Triones/Flyidea  
// Tested on Triones

static BLEUUID MAIN_SERVICE("0000ffd5-0000-1000-8000-00805f9b34fb");
static BLEUUID WRITE_CHAR("0000ffd9-0000-1000-8000-00805f9b34fb");

#define SMOOTH_RAINBOW      37
#define PULSATING_RED       38
#define PULSATING_GREEN     39
#define PULSATING_BLUE      40
#define PULSATING_YELLOW    41
#define PULSATING_CYAN      42
#define PULSATING_MAGENTA   43
#define PULSATING_WHITE     44
#define PULSATING_RED_GREEN 45
#define PULSATING_RED_BLUE  46 
#define PULSATING_GREEN_BLUE 47
#define RAINBOW_STROBE      48
#define RED_STROBE          49
#define GREEN_STROBE        50
#define BLUE_STROBE         51
#define YELLOW_STROBE       52
#define CYAN_STROBE         53
#define MAGENTA_STROBE      54
#define WHITE_STROBE        55
#define HARD_RAINBOW        56
#define PULSATING_RGB       97
#define RGB_STROBE          98
#define HARD_RGB            99
    
class BLELight {
  public:
    BLELight();
    ~BLELight();
    bool connect(BLEAddress pAddress); 
    void turnOn(); 
    void turnOff(); 
    void setColor(uint8_t r, uint8_t g, uint8_t b);//, uint8_t brightness, bool warmWhite); 
    void disconnect();
    void setMode(uint8_t mode, uint8_t speed);
    
  private:    
    bool isConnected = false; 
    BLEClient*  pClient;
    BLERemoteCharacteristic *pRemoteCharacteristic;
};


BLELight::BLELight() {    
  
}

bool BLELight::connect(BLEAddress pAddress){    
    pClient  = BLEDevice::createClient();
    Serial.println("[BLELight] - Connecting.. ");    
    pClient->connect(pAddress);

    if(pClient->isConnected()) {
      Serial.println("[BLELight] - Connected to BLE Light");
    } else {
      Serial.println("[BLELight] - Connection failed!");
      pClient->disconnect();
      return false;  
    }
    
    Serial.println("XXX");
    delay(1000); // https://github.com/nkolban/esp32-snippets/issues/874#issuecomment-944862272
    
    BLERemoteService *pRemoteService = pClient->getService(MAIN_SERVICE);
        
    if (pRemoteService == nullptr) {
      Serial.println("[BLELight] - Failed to find RGB service!");
      pClient->disconnect();
      return false;
    } 

    pRemoteCharacteristic = pRemoteService->getCharacteristic(WRITE_CHAR);
    if (pRemoteCharacteristic == nullptr)
    {
      Serial.println("[BLELight] - Failed to find WRGB_UUID characteristic!");
      pClient->disconnect();
      return false;
    } 

    isConnected = true;
    Serial.println("BLELight::connect true");
    return true; 
}

void BLELight::turnOn(){   
    if(isConnected) {
        uint8_t payload[] = {0xCC, 0x23, 0x33};
        pRemoteCharacteristic->writeValue(payload, 3);        
        Serial.println("[BLELight] - Turn on command sent!");
    } else {
      Serial.println("[BLELight] - Not connected!");
    }
}

void BLELight::turnOff(){   
    if(isConnected) {
        uint8_t payload[] = {0xCC, 0x24, 0x33};
        pRemoteCharacteristic->writeValue(payload, 3);
        Serial.println("[BLELight] - Turn off command sent!");
    } else {
      Serial.println("[BLELight] - Not connected!");
    }
}

void BLELight::setColor(uint8_t r, uint8_t g, uint8_t b){   
    if(isConnected) {
      uint8_t payload[7];

      Serial.printf("[BLELight] - Set color [R: %d G: %d B:%d] \r\n", r, g, b);
      
      payload[0] = 0x56;
      payload[1] = r; //R
      payload[2] = g; //G
      payload[3] = b; // B
      payload[4] = 0x00; //brightness; //brightness
      payload[5] = 0xF0;  
      payload[6] = 0xAA;    
      
      pRemoteCharacteristic->writeValue(payload, 7);
    } else {
      Serial.println("[BLELight] - Not connected!");
    }
}

void BLELight::disconnect(){   
    pRemoteCharacteristic = NULL;
    
    pClient->disconnect();
    pClient = NULL;
}

void BLELight::setMode(uint8_t mode, uint8_t speed) {
  if(isConnected) {
    Serial.printf("[BLELight] - Set mode %d, speed:%d\r\n", mode, speed);
    
    uint8_t payload[4];
    payload[0] = 0xBB;
    payload[1] = mode;
    payload[2] = speed;
    payload[3] = 0x44;
    pRemoteCharacteristic->writeValue(payload, 4);
  } else {
    Serial.println("[BLELight] - Not connected!");
  }
}

BLELight::~BLELight(){ 
}

#endif
