#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myServo;

const int gasPin = A0;
const int firePin = A1;
const int servoPin = 9;
const int fanPin = 10; 

const int gasThreshold = 300;
const int fireThreshold = 500;

const int gasAngle = 135;
const int fireAngle = 45;
const int centerAngle = 90;

const int fanMaxRPM = 4000;  
int rpm = 0;                 
int fanSpeed = 0;          

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Gas:    Fire:   ");
  
  myServo.attach(servoPin);
  myServo.write(centerAngle);
  
  
  pinMode(fanPin, OUTPUT);
  analogWrite(fanPin, 0);
}

void loop() {
  int gasValue = analogRead(gasPin);
  int fireValue = 1023 - analogRead(firePin);
  
  bool alertActive = false;


if (fireValue > fireThreshold) {
    myServo.write(fireAngle);
    rpm = 4000;  
    alertActive = true;
    lcd.setCursor(0, 1);
    lcd.print(" FIRE DETECTED");
}
else if (gasValue > gasThreshold) {
    myServo.write(gasAngle);
    rpm = 3000;  
    alertActive = true;
    lcd.setCursor(0, 1);
    lcd.print("  GAS DETECTED");
}


  // Normal state
  if (!alertActive) {
    myServo.write(centerAngle);
    rpm = 0; // Fan off
    lcd.setCursor(0, 1);
    lcd.print("NORMAL   ");
  }

  // Convert RPM to PWM and apply
  fanSpeed = map(rpm, 0, fanMaxRPM, 0, 255);
  analogWrite(fanPin, fanSpeed);

  // Display
  lcd.setCursor(4, 0);
  lcd.print("    ");
  lcd.setCursor(4, 0);
  lcd.print(gasValue);
  lcd.setCursor(12, 0);
  lcd.print("    ");
  lcd.setCursor(12, 0);
  lcd.print(fireValue);
  lcd.setCursor(12, 1);

  // Debug
  Serial.print("Gas: "); Serial.print(gasValue);
  Serial.print(" | Fire: "); Serial.print(fireValue);
  Serial.print(" | RPM: "); Serial.print(rpm);
  Serial.print(" | PWM: "); Serial.println(fanSpeed);

  delay(500);
}
