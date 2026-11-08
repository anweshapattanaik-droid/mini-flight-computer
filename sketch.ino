#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_MPU6050 mpu;

const int led = 13, btn = 7;
const float LAUNCH_THRESHOLD = 15.0;  // m/s2, about 1.5 g
const float WARN_LIMIT = 18.0;        // m/s2, buzzer sounds above this
const int buzzer = 8;

enum State { IDLE, ARMED, LAUNCHED };
State state = IDLE;
int prevBtn = HIGH;
unsigned long lastUpdate = 0;
const char* names[] = {"IDLE    ", "ARMED   ", "LAUNCHED"};

void showState() {
  lcd.setCursor(0, 0);
  lcd.print("State: ");
  lcd.print(names[state]);
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  if (!mpu.begin()) {
    lcd.print("MPU6050 not found");
    while (1) delay(10);
  }
  showState();
}

void loop() {
  int b = digitalRead(btn);
  if (b == LOW && prevBtn == HIGH) {
    if (state == IDLE) state = ARMED;
    else state = IDLE;          // reset from ARMED or LAUNCHED
    showState();
  }
  prevBtn = b;

  if (millis() - lastUpdate >= 200) {
    lastUpdate = millis();

    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    float acc = sqrt(a.acceleration.x * a.acceleration.x +
                     a.acceleration.y * a.acceleration.y +
                     a.acceleration.z * a.acceleration.z);

    if (state == ARMED && acc > LAUNCH_THRESHOLD) {
      state = LAUNCHED;
      showState();
    }

    digitalWrite(led, state == LAUNCHED);

    if (acc > WARN_LIMIT) tone(buzzer, 1000);
    else noTone(buzzer);

    lcd.setCursor(0, 1);
    lcd.print("Acc:");
    lcd.print(acc, 1);
    lcd.print(" m/s2  ");

    Serial.print(millis());
    Serial.print(",");
    Serial.print(names[state]);
    Serial.print(",");
    Serial.println(acc);
  }
  delay(20);
}
