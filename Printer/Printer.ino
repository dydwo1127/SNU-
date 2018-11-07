#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WebSocketsServer.h>
#include <QueueList.h>


ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

QueueList <String> textQueue;

int totalRowsToPrint = 0;
int maxRow = 20;
bool isPrinting = false;

/*__________________________________________________________SETUP__________________________________________________________*/

void setup() {

  Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println("\r\n");

  startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
  
  startSPIFFS();               // Start the SPIFFS and list all contents

  startWebSocket();            // Start a WebSocket server

  startServer();               // Start a HTTP server with a file read handler and an upload handler
  
}

/*__________________________________________________________LOOP__________________________________________________________*/

void loop() {
  webSocket.loop();
  server.handleClient();
  if(!isPrinting && !textQueue.isEmpty()){
    Print(textQueue.pop());
  }
}

/*__________________________________________________________SETUP_FUNCTIONS__________________________________________________________*/

void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection

  wifiMulti.addAP("steam2.4", "a112857h");   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }
  Serial.println("\r\n");
  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());             // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
  } else {                                   // If a station is connected to the ESP SoftAP
    Serial.print("Station connected to ESP8266 AP");
  }
  Serial.println("\r\n");
}

void startSPIFFS() { // Start the SPIFFS and list all contents
  SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {                      // List the file system contents
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started.");
}

void startServer() { // Start a HTTP server with a file read handler and an upload handler
  server.on("/edit.html",  HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
    server.send(200, "text/plain", ""); 
  });                       // go to 'handleFileUpload'

  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
                                              // and check if the file exists

  server.begin();                             // start the HTTP server
  Serial.println("HTTP server started.");
}

/*__________________________________________________________SERVER_HANDLERS__________________________________________________________*/

void handleNotFound(){ // if the requested file or page doesn't exist, return a 404 not found error
  if(!handleFileRead(server.uri())){          // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  //Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    ///////////////////////////////////Serial.println(String("\tSent file: ") + path);
    return true;
  }
  ///////////////////////////////////////////////////////////////////Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
     // Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        //IPAddress ip = webSocket.remoteIP(num);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      String words = "";
      String words_ = (char *) payload;
      for(int i = 0; i<words_.length();i++){
        if(words_[i] == '&'){
          break;
        }
        else{
          words += words_[i];
        }
      }
      if(words.length()>20){
        Serial.print("New Client(client num : ");
        Serial.print(num);
        Serial.print(") - ");
        Serial.println(words_);
        break;
      }
      else{
        if(words == "#paperOut"){
          textQueue.push(words);
        }
        else if(totalRowsToPrint<maxRow){
          totalRowsToPrint ++;
          textQueue.push(words);
          Serial.print("################################");
          Serial.println(totalRowsToPrint);
        }
        else{
          Serial.println("Paper is full. Use another paper.");
          webSocket.sendTXT(num, "#paperIsFull");
        }
        break;
      }
      break;
  }
}

/*__________________________________________________________HELPER_FUNCTIONS__________________________________________________________*/

String formatBytes(size_t bytes) { // convert sizes in bytes to KB and MB
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
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

