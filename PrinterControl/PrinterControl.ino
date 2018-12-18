#include <SoftwareSerial.h>
#include <AFMotor.h>

AF_Stepper stepper1(200, 1);
AF_Stepper stepper2(200, 2);

bool isPrinting = false;

void setup() {
  
  Serial.begin(115200);
  pinMode(10, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(2, LOW);
  digitalWrite(9, LOW);

  stepper1.setSpeed(120);
  stepper2.setSpeed(100);
  stepper1.release();
  stepper2.release();
}

void loop() {
  if(Serial.available()){
    isPrinting = true;
    digitalWrite(2, HIGH);
    String words;
    while(Serial.available()){
      words += (char)Serial.read();
      delay(5);
    }
    if(words == "#paperIn"){
      stepper2.step(270, BACKWARD, DOUBLE);
    }
    else if(words == "#paperOut"){
      stepper2.step(600, BACKWARD, DOUBLE);
      digitalWrite(9, HIGH);
      delay(10);
      digitalWrite(9, LOW);
    }
    else{
      wordsToDot(words);
    }
    isPrinting = false;
    digitalWrite(2, LOW);
  }
}
