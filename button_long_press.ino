#include <OneButton.h>


OneButton button(A1, true);

void setup(){
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  button.attachLongPressStop(longclick);
    }

void loop(){
  button.tick();
  delay(10);
}

void doubleclick(){
  digitalWrite(11, HIGH);
  delay(2000);
  digitalWrite(11, LOW);
    }

void singleclick(){
  digitalWrite(12, HIGH);
  delay(2000);
  digitalWrite(12, LOW);
    }

void longclick(){
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
    }
