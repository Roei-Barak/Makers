#include <WiFi.h>
#include <esp_now.h>

#define BUTTON_1_PIN 25
#define BUTTON_2_PIN 26
#define BUTTON_3_PIN 27

uint8_t slaveMac[] = {0x10, 0x52, 0x1C, 0x66, 0x58, 0xA4}; // Replace with your Slave MAC address

typedef struct {
  int messageID; // 1 for Forward, 2 for Backward, 3 for Turn, 0 for Clear Screen
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

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, slaveMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Master is ready.");
}

void loop() {
  int currentMessageID = 0; // Default to no button pressed (Clear Screen)

  // Read the button states and assign appropriate message IDs
  if (digitalRead(BUTTON_1_PIN)) { // Button 1 pressed
    currentMessageID = 1;
  } else if (digitalRead(BUTTON_2_PIN)) { // Button 2 pressed
    currentMessageID = 2;
  } else if (digitalRead(BUTTON_3_PIN)) { // Button 3 pressed
    currentMessageID = 3;
  } else {
    currentMessageID = 0; // No button pressed (Clear Screen)
  }

  // Only send a message if the message ID has changed
  if (currentMessageID != lastMessageID) {
    msg.messageID = currentMessageID;
    esp_now_send(slaveMac, (uint8_t *)&msg, sizeof(msg));

    // Debugging: Log the message ID being sent
    Serial.print("Sent Message ID: ");
    Serial.println(currentMessageID);

    lastMessageID = currentMessageID; // Update the last message ID
  }

  delay(100); // Small delay to debounce the buttons
}
