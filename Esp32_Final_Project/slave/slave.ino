#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <esp_now.h>

#define I2C_ADDR 0x27 // Adjust if your LCD has a different I2C address
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

// Define the structure for incoming data
typedef struct {
  int messageID; // 1 for Forward, 2 for Backward, 3 for Turn, 0 for Clear Screen
} ButtonMessage;

// Callback function for receiving ESP-NOW messages
void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  ButtonMessage msg;
  memcpy(&msg, incomingData, sizeof(msg));

  // Debugging: Print the incoming message
  Serial.print("Received Message ID: ");
  Serial.println(msg.messageID);

  // Clear the LCD if no button is pressed
  if (msg.messageID == 0) {
    Serial.println("Clearing the LCD display.");
    lcd.clear();
    return;
  }

  // Update the LCD display with the corresponding message
  lcd.clear(); // Clear the display before showing a new message
  switch (msg.messageID) {
    case 1:
      Serial.println("Displaying: Forward");
      lcd.print("Forward");
      break;
    case 2:
      Serial.println("Displaying: Backward");
      lcd.print("Backward");
      break;
    case 3:
      Serial.println("Displaying: Turn");
      lcd.print("Turn");
      break;
    default:
      Serial.println("Displaying: Unknown Command");
      lcd.print("Unknown Command");
  }
}

void setup() {
  Serial.begin(115200); // Start Serial communication for debugging
  Serial.println("Initializing Slave...");

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize WiFi in STA mode
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi set to STA mode.");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }
  Serial.println("ESP-NOW Initialized.");

  // Register the callback function
  esp_now_register_recv_cb(onReceive);
  Serial.println("ESP-NOW receive callback registered.");

  Serial.println("Slave is ready.");
}

void loop() {
  // Nothing to do here; ESP-NOW handles communication
}
