#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing Wi-Fi...");
  WiFi.mode(WIFI_STA);  // Set Wi-Fi to Station mode
  Serial.println("Wi-Fi mode set to STA.");
  WiFi.disconnect();    // Disconnect Wi-Fi to avoid interference
  delay(100);           // Wait briefly for initialization
  Serial.println("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // Nothing to do here
}
