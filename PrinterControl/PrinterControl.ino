void setup() {
  Serial.begin(115200);
}

void loop() {
  
}

String ReceiveData(){
  String words;
  while(Serial.available(){
    words += Serial.read();
    delay(2);
  }
  return words;
}

void wordToBraille(String words){
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
}

void Print(String textLine){
  isPrinting = true;
  if(textLine == "#paperOut"){
    totalRowsToPrint = 0;
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
}
