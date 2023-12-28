#include <Arduino.h>

// Based on https://randomnerdtutorials.com/esp-now-many-to-one-esp8266-nodemcu/

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <espnow.h>
#elif ESP32
#include <WiFi.h>
#include <esp_now.h>
#endif
// #include <ESPNowW.h> // TODO: does this library actually give us any benefit?

// TODO: Wifi AP for configuration and logging
// TODO: Web server - https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/
// TODO: ESP32 for keyboard HID
// TODO: OR AHK read serial port?
//    - https://www.autohotkey.com/boards/viewtopic.php?t=107692
//    - https://www.autohotkey.com/boards/viewtopic.php?t=90173
//    - https://www.autohotkey.com/board/topic/66464-receiving-rs232-data-from-serial-port/
//    - https://www.autohotkey.com/board/topic/26231-serial-com-port-console-script/#187247
// TODO: map mac addresses to keyboard keys
//    - (OLD) https://randomnerdtutorials.com/esp32-flash-memory/
//    - https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
//    - ?? compatible ?? 

String magicString = "HavingFun@OSTT";

void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

  char* buff = (char*) incomingData;
  String buffStr = String(buff);
  if (buffStr == magicString) {
    Serial.print("Valid packet received from: ");
    Serial.println(macStr);
  } else {
    Serial.print("Spurious packet received from: ");
    Serial.print(macStr);
    Serial.printf(" '%s'", buffStr);
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Button Receiver Startup");
  Serial.print("ESP Board MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // No loop because we are doing everything in the OnDataRecv function
}
