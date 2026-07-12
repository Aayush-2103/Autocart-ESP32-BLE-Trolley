#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
// ---------------- MOTOR PINS ----------------
#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26
#define ENA 14
#define ENB 27
// ---------------- IR SENSORS ----------------
#define IR_LEFT  23
#define IR_RIGHT 22
// ---------------- BLE ----------------
#define TARGET_UUID "c001c001-c001-c001-c001-c001c001c001"
BLEScan* pBLEScan;
// ---------------- SPEED ----------------
#define SPEED_FAST 240
#define SPEED_SLOW 120
// ---------------- FILTER ----------------
int leftCount = 0;
int rightCount = 0;
#define DETECT_THRESHOLD 3
// ---------------- TIMING ----------------
unsigned long lastBLEScan = 0;
unsigned long lastSeenBLE = 0;
// STOP FEATURE
bool stopActive = false;
unsigned long stopStartTime = 0;
#define STOP_TIME 1000   // 1 second
bool deviceFound = false;
// ---------------- BLE CALLBACK ----------------
class MyCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice device) {
    if (device.haveServiceUUID() &&
        device.getServiceUUID().toString() == TARGET_UUID) {
      deviceFound = true;
      lastSeenBLE = millis();
      Serial.println("✅ BLE FOUND");
    }
  }
};
// ---------------- MOTOR FUNCTIONS ----------------
void stopCart() {
  ledcWrite(ENA, 0);
  ledcWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, SPEED_FAST);
  ledcWrite(ENB, SPEED_FAST);
}
// SOFT LEFT TURN
void curveLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, SPEED_FAST);
  ledcWrite(ENB, SPEED_SLOW);
}
// SOFT RIGHT TURN
void curveRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(ENA, SPEED_SLOW);
  ledcWrite(ENB, SPEED_FAST);
}
// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  ledcAttach(ENA, 800, 8);
  ledcAttach(ENB, 800, 8);
  stopCart();

  // Safe startup
  lastSeenBLE = millis();
  deviceFound = false;
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("🚀 FINAL SYSTEM READY");
}
// ---------------- LOOP ----------------
void loop() {
  // BLE SCAN
  if (millis() - lastBLEScan > 1500) {
    lastBLEScan = millis();
    deviceFound = false;
    pBLEScan->start(1, false);
    pBLEScan->clearResults();
  }
  // STOP if BLE not found
  if (!deviceFound || millis() - lastSeenBLE > 2000) {
    stopCart();
    Serial.println("🛑 WAITING FOR BLE");
    return;
  }

  // ---------------- SENSOR FILTER ----------------
  bool leftRaw  = (digitalRead(IR_LEFT)  == LOW);
  bool rightRaw = (digitalRead(IR_RIGHT) == LOW);
  if (leftRaw) leftCount++;
  else leftCount = 0;
  if (rightRaw) rightCount++;
  else rightCount = 0;
  bool left  = (leftCount >= DETECT_THRESHOLD);
  bool right = (rightCount >= DETECT_THRESHOLD);
  Serial.print("L:");
  Serial.print(left);
  Serial.print(" R:");
  Serial.println(right);
  // ---------------- BOTH SENSOR STOP ----------------
  // Start stop
  if (left && right && !stopActive) {
    stopActive = true;
    stopStartTime = millis();
    stopCart();
    Serial.println("🛑 BOTH - STOPPING");
    return;
  }
  // During stop
  if (stopActive) {
    if (millis() - stopStartTime < STOP_TIME) {
      stopCart();
      return;
    } else {
      stopActive = false;
    }
  }
  // ---------------- MOVEMENT ----------------
  if (left && !right) {
    curveLeft();
    delay(30);
    return;
  }
  if (right && !left) {
    curveRight();
    delay(30);
    return;
  }
  forward();
  delay(30);
}
