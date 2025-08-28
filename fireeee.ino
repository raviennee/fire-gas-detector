#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myServo;

//Pin
const int gasPin = A0;
const int firePin = A1;
const int servoPin = 9;
const int fanPin = 10; 
const int ledRed = 2;
const int ledYellow = 3;
const int ledGreen = 4;

//Threshold
const int gasThreshold = 300;
const int fireThreshold = 500;

//Servo posisi
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

  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledGreen, HIGH);
}

void loop() {
  int gasValue = analogRead(gasPin);
  int fireValue = 1023 - analogRead(firePin);
  
  bool alertActive = false;

  //prioritas api>gas
  if (fireValue > fireThreshold && gasValue > gasThreshold) {
    myServo.write(fireAngle);
    digitalWrite(fanPin, LOW);
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print(" ALL DETECTED  ");
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, LOW);
  }
  else if (fireValue > fireThreshold) {
    myServo.write(fireAngle);
    digitalWrite(fanPin, LOW);
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print(" FIRE DETECTED ");
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, LOW);
  }
  else if (gasValue > gasThreshold) {
    myServo.write(gasAngle);
    digitalWrite(fanPin, LOW);
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print("  GAS DETECTED ");
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen, LOW);
  }
  else {
    // NORMAL
    digitalWrite(fanPin, HIGH); //kipas off
  }

  if (!alertActive) {
    myServo.write(centerAngle);
    lcd.setCursor(0, 1);
    lcd.print("     NORMAL    ");
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledGreen, HIGH);
  }

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
