
#include <EEPROM.h>

#define POT_PIN      34  // Chiết áp
#define GEAR_PIN     32  // Nút gạt tiến/lùi
#define RESET_ODO    25  // Nút reset ODO
#define RESET_TRIP   26  // Nút reset Trip

int speed = 0;
int rpm = 0;
float odo = 0.0;
float trip = 0.0;
bool gearForward = true;
bool lastGearState = HIGH;

unsigned long lastSend = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(GEAR_PIN, INPUT_PULLUP);
  pinMode(RESET_ODO, INPUT_PULLUP);
  pinMode(RESET_TRIP, INPUT_PULLUP);

  EEPROM.begin(512);
  EEPROM.get(0, odo);
  if (isnan(odo)) odo = 0;

  EEPROM.get(10, trip);
  if (isnan(trip)) trip = 0;

  lastTime = millis();
}

void loop() {
  // Đọc giá trị chiết áp
  int analogVal = analogRead(POT_PIN);
  speed = map(analogVal, 0, 4095, 0, 120);   // Giới hạn tốc độ 0-220 km/h
  rpm = map(analogVal, 0, 4095, 0, 8);       // RPM x1000

  // Tính quãng đường đi được
  unsigned long now = millis();
  float elapsedTime = (now - lastTime) / 1000.0; // giây
  lastTime = now;
  float distance = speed * (elapsedTime / 3600.0); // km = km/h * h
  odo += distance;
  trip += distance;

  // Lưu ODO & Trip sau mỗi 5 giây
  if (now - lastSend >= 5000) {
    EEPROM.put(0, odo);
    EEPROM.put(10, trip);
    EEPROM.commit();
    lastSend = now;
  }

  // Đọc nút reset ODO
  if (digitalRead(RESET_ODO) == LOW) {
    odo = 0;
    EEPROM.put(0, odo);
    EEPROM.commit();
    delay(300);
  }

  // Đọc nút reset Trip
  if (digitalRead(RESET_TRIP) == LOW) {
    trip = 0;
    EEPROM.put(10, trip);
    EEPROM.commit();
    delay(300);
  }

  // Đọc trạng thái số tiến/lùi
  bool currentGearState = digitalRead(GEAR_PIN);
  if (currentGearState != lastGearState) {
    gearForward = !gearForward;
    lastGearState = currentGearState;
    delay(200);
  }

  // Gửi dữ liệu lên Qt qua Serial
  Serial.printf("%d,%d,%.0f,%.0f,%s\n", speed, rpm, odo, trip, gearForward ? "D" : "R");

  delay(100);
}
