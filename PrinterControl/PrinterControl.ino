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
  /*if(Serial.available()){
    while(Serial.available()){
      Serial.read();
    }
  }
  delay(50);*/
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

/*void wordToBraille(String words){
  int wordLength = words.length();
  int codeArray[wordLength][6];
  for(int i = 0; i<wordLength; i++){
    wordToCode(codeArray[i],words[i]);
  }
  for(int i = 0; i<wordLength; i++){
    for(int j = 0; j<6; j++){
      Serial.print(codeArray[i][j]);
    }
  }
  Serial.println();
  for(int i = 0; i<wordLength; i++){
    Serial.print(codeArray[i][0]);
    Serial.print(codeArray[i][1]);
  }
  Serial.println();
  for(int i = 0; i<wordLength; i++){
    Serial.print(codeArray[i][2]);
    Serial.print(codeArray[i][3]);
  }
  Serial.println();
  for(int i = 0; i<wordLength; i++){
    Serial.print(codeArray[i][4]);
    Serial.print(codeArray[i][5]);
  }
  Serial.println();
}*/

/*void Print(String textLine){
  isPrinting = true;
  if(textLine == "#paperOut"){
    Serial.println("Paper Out");
    isPrinting = false;
    return;
  }
  Serial.println(textLine);
  wordToBraille(textLine);
  for(int i =0; i<10;i++){
    Serial.print(".");
    delay(100);
  }
  Serial.println("done");
  isPrinting = false;
}*/
