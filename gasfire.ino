#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

const int sensorPins[2] = {A0, A1};  //[0] = gas, [1] = fire
int sensorValues[2];                 //nilai bacaan sensor

const int servoPin = 9;
const int fanPin = 10;
const int leds[3] = {2, 3, 4}; // [0]=red, [1]=yellow, [2]=green

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
  digitalWrite(fanPin, HIGH); //kipas off

  for (int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
  }

  digitalWrite(leds[0], LOW);   //merah
  digitalWrite(leds[1], LOW);   //kuning
  digitalWrite(leds[2], HIGH);  //hijau (safe)
}

void loop() {
  sensorValues[0] = analogRead(sensorPins[0]);       //Gas
  sensorValues[1] = 1023 - analogRead(sensorPins[1]); //Fire (dibalik)

  bool alertActive = false;

  //prioritas api>gas
  if (sensorValues[1] > fireThreshold && sensorValues[0] > gasThreshold) {
    myServo.write(fireAngle);
    digitalWrite(fanPin, LOW); //Kipas on
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print(" ALL DETECTED  ");
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], LOW);
  }
  else if (sensorValues[1] > fireThreshold) {
    myServo.write(fireAngle);
    digitalWrite(fanPin, LOW); //Kipas on
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print(" FIRE DETECTED ");
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], LOW);
    digitalWrite(leds[2], LOW);
  }
  else if (sensorValues[0] > gasThreshold) {
    myServo.write(gasAngle);
    digitalWrite(fanPin, LOW); //Kipas on
    alertActive = true;

    lcd.setCursor(0, 1);
    lcd.print("  GAS DETECTED ");
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[1], HIGH);
    digitalWrite(leds[2], LOW);
  }

  //NORMAL
  if (!alertActive) {
    myServo.write(centerAngle);
    digitalWrite(fanPin, HIGH); //Kipas off

    lcd.setCursor(0, 1);
    lcd.print("     NORMAL    ");
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[1], LOW);
    digitalWrite(leds[2], HIGH);
  }

  lcd.setCursor(4, 0);
  lcd.print("    ");
  lcd.setCursor(4, 0);
  lcd.print(sensorValues[0]);

  lcd.setCursor(12, 0);
  lcd.print("    ");
  lcd.setCursor(12, 0);
  lcd.print(sensorValues[1]);

  Serial.print("Gas: "); Serial.print(sensorValues[0]);
  Serial.print(" | Fire: "); Serial.println(sensorValues[1]);

  delay(500);
}
