#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myServo;

// Pin
const int gasPin = A0;
const int firePin = A1;
const int servoPin = 9;
const int fanPin = 10; 

// Array untuk LED
const int ledPins[] = {2, 3, 4}; // {Red, Yellow, Green}
enum {LED_RED, LED_YELLOW, LED_GREEN}; // biar gampang pakai nama

// Threshold
const int gasThreshold = 300;
const int fireThreshold = 500;

// Servo posisi
const int gasAngle = 160;
const int fireAngle = 30;
const int centerAngle = 97;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Gas:    Api:");
  
  myServo.attach(servoPin);
  myServo.write(centerAngle);
  
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, HIGH); // kipas off (default)

  // set semua LED jadi output
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(ledPins[LED_GREEN], HIGH); // awal hijau (safe)
}

void setLeds(bool red, bool yellow, bool green) {
  digitalWrite(ledPins[LED_RED], red ? HIGH : LOW);
  digitalWrite(ledPins[LED_YELLOW], yellow ? HIGH : LOW);
  digitalWrite(ledPins[LED_GREEN], green ? HIGH : LOW);
}

void loop() {
  int gasValue = analogRead(gasPin);
  int fireValue = 1023 - analogRead(firePin);
  
  bool alertActive = false;

  // PRIORITAS API DULUAN
  if (fireValue > fireThreshold && gasValue > gasThreshold) {
    myServo.write(fireAngle);
    digitalWrite(fanPin, LOW); // nyalakan kipas
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print(" ALL DETECTED  ");
    setLeds(true, true, false); // merah + kuning
  }
  else if (fireValue > fireThreshold) {
    myServo.write(fireAngle);
    digitalWrite(fanPin, LOW); 
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print(" FIRE DETECTED ");
    setLeds(true, false, false);
  }
  else if (gasValue > gasThreshold) {
    myServo.write(gasAngle);
    digitalWrite(fanPin, LOW); 
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print("  GAS DETECTED ");
    setLeds(false, true, false);
  }
  else {
    // NORMAL
    digitalWrite(fanPin, HIGH); // kipas off
  }

  if (!alertActive) {
    myServo.write(centerAngle);
    lcd.setCursor(0, 1);
    lcd.print("     NORMAL    ");
    setLeds(false, false, true);
  }

  // Tampilkan data sensor
  lcd.setCursor(4, 0);
  lcd.print("    ");
  lcd.setCursor(4, 0);
  lcd.print(gasValue);

  lcd.setCursor(12, 0);
  lcd.print("    ");
  lcd.setCursor(12, 0);
  lcd.print(fireValue);

  Serial.print("Gas: "); Serial.print(gasValue);
  Serial.print(" | Fire: "); Serial.println(fireValue);

  delay(500);
}
