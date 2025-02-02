#include <WiFi.h>
#include <esp_now.h>

#define BUTTON_1_PIN 21
#define BUTTON_2_PIN 19
#define BUTTON_3_PIN 18

uint8_t slaveMac[] = {0x10, 0x52, 0x1C, 0x66, 0x58, 0xA4}; // Replace with your Slave MAC

typedef struct {
  int messageID; // 1 for Forward, 2 for Backward, 3 for Spin
} ButtonMessage;

ButtonMessage msg;
int lastMessageID = -1; // Track the last message sent

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, slaveMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA; // Fix for proper ESP-NOW peer setup

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("ESP-NOW Master ready.");
}

void loop() {
  int currentMessageID = 0; // Default to no button pressed

  if (digitalRead(BUTTON_1_PIN) == HIGH) { 
    currentMessageID = 1; 
    Serial.println("Forward Command Sent");
  } else if (digitalRead(BUTTON_2_PIN) == HIGH) { 
    currentMessageID = 2; 
    Serial.println("Backward Command Sent");
  } else if (digitalRead(BUTTON_3_PIN) == HIGH) { 
    currentMessageID = 3; 
    Serial.println("Spin Command Sent");
  } else {
    currentMessageID = 0; 
    Serial.println("No Button Pressed (Clear Screen)");
  }

  if (currentMessageID != lastMessageID) {
    msg.messageID = currentMessageID;
    esp_now_send(slaveMac, (uint8_t *)&msg, sizeof(msg));
    lastMessageID = currentMessageID;
  }

  delay(100); // Debounce
}
