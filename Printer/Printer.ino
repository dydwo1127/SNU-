#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WebSocketsServer.h>
#include <QueueList.h>
#include <SoftwareSerial.h>


ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

QueueList<String> textQueue;
QueueList<String> allText;

SoftwareSerial BTSerial(2,0);

int totalRowsToPrint = 0;
int maxRow = 10;

/*__________________________________________________________SETUP__________________________________________________________*/

void setup() {

  Serial.begin(115200);        // Start the Serial communication to send messages to the computer
  delay(10);
  BTSerial.begin(9600);
//  Serial.println("\r\n");

  pinMode(5, INPUT);
  pinMode(4, INPUT);

  startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
  
  startSPIFFS();               // Start the SPIFFS and list all contents

  startWebSocket();            // Start a WebSocket server

  startServer();               // Start a HTTP server with a file read handler and an upload handler
  
}

/*__________________________________________________________LOOP__________________________________________________________*/

void loop() {
  webSocket.loop();
  server.handleClient();
  sendWordsToPrinter();
  if(digitalRead(4)){
    totalRowsToPrint = 0;
    for(int i = 0; i < allText.count(); i++){
      allText.pop();
    }
    webSocket.broadcastTXT("@empty");
  }
}

/*__________________________________________________________SETUP_FUNCTIONS__________________________________________________________*/

void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection

  wifiMulti.addAP("VanHot3", "choi8841");   // add Wi-Fi networks you want to connect to

//  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1) {  // Wait for the Wi-Fi to connect
    delay(250);
//    Serial.print('.');
  }
//  Serial.println("\r\n");
  if(WiFi.softAPgetStationNum() == 0) {      // If the ESP is connected to an AP
//    Serial.print("Connected to ");
//    Serial.println(WiFi.SSID());             // Tell us what network we're connected to
//    Serial.print("IP address:\t");
    BTSerial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
  } else {                                   // If a station is connected to the ESP SoftAP
//    Serial.print("Station connected to ESP8266 AP");
  }
//  Serial.println("\r\n");
}

void startSPIFFS() { // Start the SPIFFS and list all contents
  SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
//  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {                      // List the file system contents
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
//      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
//    Serial.printf("\n");
  }
}

void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
//  Serial.println("WebSocket server started.");
}

void startServer() { // Start a HTTP server with a file read handler and an upload handler
  server.on("/edit.html",  HTTP_POST, []() {  // If a POST request is sent to the /edit.html address,
    server.send(200, "text/plain", ""); 
  });                       // go to 'handleFileUpload'

  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
                                              // and check if the file exists

  server.begin();                             // start the HTTP server
//  Serial.println("HTTP server started.");
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
        for(int i = 0; i < allText.count(); i++){
          String temp = allText.pop();
          webSocket.sendTXT(num, temp);
          allText.push(temp);
        }
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      String words = (char *)payload;
      handlePayload(num, words);
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

void handlePayload(int num, String words){
    if(words.length() == 0){
      return;
    }
    else if(words == "#paperIn"){
      textQueue.push(words);
    }
    else if(words == "#paperOut"){
      if(totalRowsToPrint != maxRow +1){
        totalRowsToPrint = maxRow + 1;
        textQueue.push(words);
        webSocket.broadcastTXT("+paperOut");
        allText.push("+paperOut");
      }
      else{
        webSocket.sendTXT(num, "!Wait unitl paper out");
      }
    }
    else if(totalRowsToPrint<maxRow){
      totalRowsToPrint ++;
      textQueue.push(words);
      webSocket.broadcastTXT("+" + words);
      allText.push("+" + words);
    }
    else if(totalRowsToPrint == maxRow +1){
      webSocket.sendTXT(num, "!Wait unitl paper out");
    }
    else{
      webSocket.sendTXT(num, "!Paper Is Full!");
    }
}

void sendWordsToPrinter(){
  if(!textQueue.isEmpty() && !digitalRead(5)){
    Serial.print(textQueue.pop());
    delay(50);
    //Serial.flush();
  }
}
