#include <Arduino.h>
#include <SoftwareSerial.h>

#define Botao 2

#define LedRed 13
#define LedGreen 12
#define LedBlue 11

const int ClickTime = 500;
const int HoldTime  = 1000;

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long buttonDownTime = 0;
bool buttonClicked = false;
bool buttonHeld = false;

SoftwareSerial esp8266(6,7);

void setup() {
  pinMode(Botao, INPUT_PULLUP);

  pinMode(LedRed, OUTPUT);
  pinMode(LedBlue, OUTPUT);
  pinMode(LedGreen, OUTPUT);

  Serial.begin(9600); 

  esp8266.begin(9600);
  delay(2000);
  Serial.println("tentando comunicar");
  esp8266.println("Conectado");

  digitalWrite(LedGreen, HIGH);
}

void loop() {

  if ( Serial.available()) {

  }

  int reading = digitalRead(Botao);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  };

  if((millis() - lastDebounceTime) > 50) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        buttonDownTime = millis();
      } else {
        if ((millis() - buttonDownTime) < ClickTime) {
          buttonClicked = true;
          digitalWrite(LedGreen, LOW);
          digitalWrite(LedBlue, HIGH);
          esp8266.write("click\n");
          Serial.println("CLICK");
          delay(1000);
        } else if ((millis() - buttonDownTime) > HoldTime) {
          buttonHeld = true;
          digitalWrite(LedGreen, LOW);
          digitalWrite(LedRed, HIGH);
          esp8266.write("hold\n");
          Serial.println("HOLD");
          delay(1000);
      }
    }
  }
}
  lastButtonState = reading;
  digitalWrite(LedGreen, HIGH);
  digitalWrite(LedBlue, LOW);
  digitalWrite(LedRed, LOW);
}