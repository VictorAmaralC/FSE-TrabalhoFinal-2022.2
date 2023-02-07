#include <Arduino.h>

#define LED        2  // GPIO2
#define PIN_BLUE   18 // GIOP18
#define PIN_GREEN  19 // GIOP19
#define PIN_RED    21 // GIOP21
#define BUZZER     22 // GPIO22

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  pinMode(BUZZER,  OUTPUT);
}

void loop() {
  printf("Pode ir.");
  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE,  0);
  delay(5000); // keep the color 5 second

  printf("Diminua a velocidade...");
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 100);
  analogWrite(PIN_BLUE,  0);
  delay(3000); // keep the color 3 second

  printf("PARE!");
  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  0);
  delay(5000); // keep the color 5 second
}
