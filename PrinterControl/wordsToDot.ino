int horVec = 0;
int verVec = 0;

void wordsToDot(String words){
  int wordLength = words.length();
  int codeArray[wordLength][6];
  for(int i = 0; i<wordLength; i++){
    wordToCode(codeArray[i],words[i]);
  }
  /*for(int i = 0; i<wordLength; i++){
    for(int j = 0; j<6; j++){
      Serial.print(codeArray[i][j]);
    }
  }
  Serial.println();*/
  for(int i = 0; i<wordLength; i++){
    makeDot(codeArray[i][0]);
    makeDot(codeArray[i][1]);
    stepper1.step(15, FORWARD, DOUBLE);
    horVec += 15;
  }
  stepper1.step(horVec, BACKWARD, DOUBLE);
  horVec = 0;
  stepper2.step(10, BACKWARD, DOUBLE);
  for(int i = 0; i<wordLength; i++){
    makeDot(codeArray[i][2]);
    makeDot(codeArray[i][3]);
    stepper1.step(15, FORWARD, DOUBLE);
    horVec += 15;
  }
  stepper1.step(horVec, BACKWARD, DOUBLE);
  horVec = 0;
  stepper2.step(10, BACKWARD, DOUBLE);
  for(int i = 0; i<wordLength; i++){
    makeDot(codeArray[i][4]);
    makeDot(codeArray[i][5]);
    stepper1.step(15, FORWARD, DOUBLE);
    horVec += 15;
  }
  stepper1.step(horVec, BACKWARD, DOUBLE);
  horVec = 0;
  stepper2.step(40, BACKWARD, DOUBLE);
}

void makeDot(int singleCode){
  if(singleCode == 0){
    stepper1.step(15, FORWARD, DOUBLE);
    horVec += 15;
  }
  if(singleCode == 1){
    digitalWrite(10, LOW);
    delay(150);
    digitalWrite(10, HIGH);
    stepper1.step(15, FORWARD, DOUBLE);
    horVec += 15;
  }
  if(singleCode == 9){
    stepper1.step(15,FORWARD, DOUBLE);
    horVec += 15;
  }
}

