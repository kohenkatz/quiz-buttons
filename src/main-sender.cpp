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

// TODO: check battery charge and change how light flashes if battery is low
// TODO: flash light for failure to send
// TODO: detect receiver's address and store in EEPROM instead of hard-coding
// TODO: get batteries - https://www.adafruit.com/category/44_138 or Amazon?
// TODO: switches for batteries ??, e.g. https://www.adafruit.com/product/3064

// Receiver MAC address
uint8_t broadcastAddress[] = {0x08, 0x3A, 0x8D, 0xCE, 0x45, 0x89};

String magicString = "HavingFun@OSTT";
uint8_t *buffer = (uint8_t*) magicString.c_str();
size_t sizeBuff = sizeof(buffer) * magicString.length();

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
    // Light LED for 3 seconds
    digitalWrite(LED_BUILTIN, LOW);
    delay(3000);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Sleeping until next button press...");
    ESP.deepSleep(0);
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Button Sender Startup");
  Serial.print("ESP Board MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Setup to light the LED when we know that our message was received
  pinMode(LED_BUILTIN, OUTPUT);
  // Start with LED off
  digitalWrite(LED_BUILTIN, HIGH);

  // Set device as a Wi-Fi Station
  #ifdef ESP8266
    WiFi.mode(WIFI_STA);
  #elif ESP32
    WiFi.mode(WIFI_MODE_STA);
  #endif
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set ESP-NOW role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  esp_now_send(0, buffer, sizeBuff);

  // Serial.print("Sleeping in 5 seconds...");
  // delay(3000);
  // ESP.deepSleep(0);
}

void loop() {
  // No loop because we are using reset pin to trigger
  // and deep sleep to save battery
}
