#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 scale;

float calibration_factor = -7050;

#define MotorPin1 10
#define MotorPin2 11
#define EnablePin1 5

#define MotorPin3 13
#define MotorPin4 12
#define EnablePin2 6

#define potPin A0

const int buttonUp = 7;
const int buttonDown = 8;
int buttonStateUp = 0;
int buttonStateDown = 0;

void setup() {
  Serial.begin(9600);

  pinMode(MotorPin1, OUTPUT);
  pinMode(MotorPin2, OUTPUT);
  pinMode(EnablePin, OUTPUT);

  pinMode(MotorPin3, OUTPUT);
  pinMode(MotorPin4, OUTPUT);
  pinMode(EnablePin, OUTPUT);

  pinMode(potPin, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);


  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();

  long zero_factor = scale.read_average();
}

void loop() {
  scale.set_scale(calibration_factor);
  Serial.print(scale.get_units(), 1);

  int potValue = 0, pwmValue = 0;
  buttonStateUp = digitalRead(buttonUp);
  if (buttonStateUp == HIGH) {
    potValue = analogRead(potPin);
    pwmValue = map(potValue, 0, 1023, 0, 254);
    if (scale.get_units() > 15) {  //threshold value
      digitalWrite(MotorPin1, HIGH);
      digitalWrite(MotorPin2, LOW);

      digitalWrite(MotorPin3, HIGH);
      digitalWrite(MotorPin4, LOW);

      analogWrite(EnablePin1, pwmValue);
      analogWrite(EnablePin2, pwmValue);
    }
    delay(100);
  }
  if (buttonStateDown == HIGH) {
    potValue = analogRead(potPin);
    pwmValue = map(potValue, 0, 1023, 0, 254);
    if (scale.get_units() > 15) {  //threshold value
      digitalWrite(MotorPin1, LOW);
      digitalWrite(MotorPin2, HIGH);

      digitalWrite(MotorPin3, LOW);
      digitalWrite(MotorPin4, HIGH);

      analogWrite(EnablePin1, pwmValue);
      analogWrite(EnablePin2, pwmValue);
    }
    delay(100);
  }
  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
}
