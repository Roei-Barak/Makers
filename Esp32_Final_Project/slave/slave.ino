#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Make sure this is ESP32-compatible
#include <WiFi.h>
#include <esp_now.h>

#define I2C_ADDR 0x27 // Adjust if needed
#define LCD_COLS 16
#define LCD_ROWS 2

#define LEFT_MOTOR_FORWARD_PIN 32
#define LEFT_MOTOR_BACKWARD_PIN 33
#define RIGHT_MOTOR_FORWARD_PIN 26
#define RIGHT_MOTOR_BACKWARD_PIN 27

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

typedef struct {
  int messageID; // 1 = Forward, 2 = Backward, 3 = Spin
} ButtonMessage;

void onReceive(const uint8_t *macAddr, const uint8_t *incomingData, int len) {
  ButtonMessage msg;
  memcpy(&msg, incomingData, sizeof(msg));

  Serial.print("Received Message ID: ");
  Serial.println(msg.messageID);

  lcd.clear(); 

  switch (msg.messageID) {
    case 1:
      Serial.println("Forward");
      lcd.print("Forward");
      digitalWrite(LEFT_MOTOR_FORWARD_PIN, HIGH);
      digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_FORWARD_PIN, HIGH);
      digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
      break;

    case 2:
      Serial.println("Backward");
      lcd.print("Backward");
      digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD_PIN, HIGH);
      digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, HIGH);
      break;

    case 3:
      Serial.println("Spin");
      lcd.print("Spin");
      digitalWrite(LEFT_MOTOR_FORWARD_PIN, HIGH);
      digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
      break;

    default:
      Serial.println("Clearing LCD");
      lcd.print("No Command");
      digitalWrite(LEFT_MOTOR_FORWARD_PIN, LOW);
      digitalWrite(LEFT_MOTOR_BACKWARD_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_FORWARD_PIN, LOW);
      digitalWrite(RIGHT_MOTOR_BACKWARD_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  pinMode(LEFT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(onReceive);
  Serial.println("ESP-NOW Receiver Ready.");
}

void loop() {
  // Nothing needed; ESP-NOW handles communication
}
