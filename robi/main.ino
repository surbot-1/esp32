#include <WiFi.h> 
#include <WiFiClient.h> 
#include <WiFiAP.h> 
#include <ESPmDNS.h> 
#include <ArduinoJson.h> 
#include <AsyncTCP.h> 
#include <ESPAsyncWebServer.h> 
const char* ssid = "WiFi_AP"; 
const char* password = "pass1234"; 
bool ledState = 0; 
char* command = ""; 
const int ledPin = 2; 

#define E1 13 
#define E2 12 
#define E3 14 
#define E4 27 

void forward() { 
    digitalWrite(E1, LOW);
    digitalWrite(E2, HIGH); 
    digitalWrite(E3, LOW); 
    digitalWrite(E4, HIGH); 
    } 
void backward() {
    digitalWrite(E1, HIGH);
    digitalWrite(E2, LOW); 
    digitalWrite(E3, HIGH); 
    digitalWrite(E4, LOW); 
    } 
void left() {
    digitalWrite(E1, HIGH);
    digitalWrite(E2, LOW); 
    digitalWrite(E3, LOW); 
    digitalWrite(E4, HIGH); 
    delay(200); 
    digitalWrite(E1, HIGH);
    digitalWrite(E2, HIGH); 
    digitalWrite(E3, HIGH); 
    digitalWrite(E4, HIGH); 
    } 
void right() {
    digitalWrite(E1, LOW);
    digitalWrite(E2, HIGH); 
    digitalWrite(E3, HIGH); 
    digitalWrite(E4, LOW); 
    delay(200); 
    digitalWrite(E1, HIGH);
    digitalWrite(E2, HIGH); 
    digitalWrite(E3, HIGH); 
    digitalWrite(E4, HIGH); 
    } 
void stop() {
    digitalWrite(E1, HIGH);
    digitalWrite(E2, HIGH); 
    digitalWrite(E3, HIGH); 
    digitalWrite(E4, HIGH); 
    } 
void setGPIOPins() {
  pinMode(E1, OUTPUT); 
  pinMode(E2, OUTPUT); 
  pinMode(E3, OUTPUT); 
  pinMode(E4, OUTPUT);  
  digitalWrite(E1, HIGH);
  digitalWrite(E2, HIGH); 
  digitalWrite(E3, HIGH); 
  digitalWrite(E4, HIGH);
} 

/* create wewebserver object */ 
AsyncWebServer server(80); 
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>  
  
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <title>WiFiRobo</title>
</head>
  
<body style= 'width:auto; height:auto; margin:0px; padding:0px; background-color:#E0E0E0'> 

  <div id='div1' style='background-color:#00A0A0; width:auto; height:auto; font-size:20px; font-weight:bold; text-align:center; color:#FFFFFF;'>
    ROBI
  </div>
  <div id='div2' style='text-align:center;'> 
    <p>Command: <span id='command'>%COMMAND%</span></p>
  </div>
  <div id='div3' style='display:flex; justify-content:center; align-items:center; margin-top:20px;'> 
  <div id='div4'>
    <button type='button' id='forward' onclick='forward()' style='margin-left:80px; margin-top:0px; width:40px; height:40px; background-color:#A0A0E0; color:#FFFFFF;'>
    <img src=''></button><br>
    <button type='button' id='left' onclick='left()' style='margin-left:0px; margin-top:40px; width:40px; height:40px; background-color:#A0A0E0; color:#FFFFFF;'>
    <img src=''></button> 
    <button type='button' id='stop' onclick='stop()' style='margin-left:40px; margin-top:40px; width:40px; height:40px; background-color:#A0A0E0; color:#FFFFFF;'>
    <img src=''></button> 
    <button type='button' id='right' onclick='right()' style='margin-left:40px; margin-top:40px; width:40px; height:40px; background-color:#A0A0E0; color:#FFFFFF;'>
    <img src=''></button><br>
    <button type='button' id='backward' onclick='backward()' style='margin-left:80px; margin-top:40px; width:40px; height:40px; background-color:#A0A0E0; color:#FFFFFF;'>
    <img src=''></button> 
    </div>
   </div> 
   
   <script>
   var gateway = `ws://${window.location.hostname}/ws`;
   var websocket;
   function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; /* <-- add this line */ 
   }
   function onOpen(event) {
    console.log('Connection opened');
   }
   function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
   }
   function onMessage(event) {
     var command;
     if (event.data == 'setting'){
       command = 'setting';
     }
     else{
       command = event.data;
     }
     document.getElementById('command').innerHTML = command;
   }
   window.addEventListener('load', onLoad);
   function onLoad(event) {
      initWebSocket();
      initButton();
   }
   function initButton() {
      document.getElementById('setting').addEventListener('click', setting); 
      document.getElementById('forward').addEventListener('click', forward); 
      document.getElementById('backward').addEventListener('click', backward); 
      document.getElementById('left').addEventListener('click', left); 
      document.getElementById('right').addEventListener('click', right); 
      document.getElementById('stop').addEventListener('click', stop); 
   }
   function toggle(){
      websocket.send('setting');
   } 
   function forward(){
      websocket.send('forward');
   } 
   function backward(){
      websocket.send('backward');
   } 
   function left(){
      websocket.send('left');
   } 
   function right(){
      websocket.send('right');
   } 
   function stop(){
      websocket.send('stop'); 
   } 
   </script>
   
</body>
</html> )rawliteral"; 

void notifyClients() { 
    ws.textAll(String(command)); 
} 
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) { 
    AwsFrameInfo *info = (AwsFrameInfo*)arg; 
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) { 
        data[len] = 0; 
        if (strcmp((char*)data, "setting") == 0) { 
            ledState = !ledState; 
            notifyClients(); 
        } 
        Serial.println((char*)data); 
        command = (char*)data; 
        if (command == "setting") { 
            setting(); notifyClients(); 
        } else if (command == "forward") { 
            forward(); notifyClients(); 
        } else if (command == "backward") { 
            backward(); notifyClients(); 
        } else if (command == "left") { 
            left(); notifyClients(); 
        } else if (command == "right") { 
            right(); notifyClients(); 
        } else if (command == "stop") { 
            stop(); notifyClients(); 
        } 
    }
} 
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { 
    switch (type) { 
        case WS_EVT_CONNECT: 
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str()); 
            break; 
        case WS_EVT_DISCONNECT: 
            Serial.printf("WebSocket client #%u disconnected\n", client->id()); 
            break; 
        case WS_EVT_DATA: 
            handleWebSocketMessage(arg, data, len); 
            break; 
        case WS_EVT_PONG: 
        case WS_EVT_ERROR: 
            break; 
    } 
} 
void initWebSocket() { 
    ws.onEvent(onEvent); 
    server.addHandler(&ws); 
} 
String processor(const String& var) { 
    Serial.println(var); 
    if(var == "COMMAND"){ 
       /* if (ledState){ 
            return "ON"; 
        } else { 
            return "OFF"; 
        } */
        return command; 
    } 
    return String(); 
} 
void setup(){ 
    /* Serial port for debugging purposes */ 
    Serial.begin(115200); 
    pinMode(ledPin, OUTPUT); 
    digitalWrite(ledPin, LOW); 
    setGPIOPins(); 
    /* Connect to Wi-Fi */ 
    /* WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.println("Connecting to WiFi.."); 
    Serial.println(WiFi.localIP()); 
    } */ 
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP); 
    if(!MDNS.begin("editapp")) { 
        Serial.println("Error starting mDNS"); 
        return; 
    } else { 
        Serial.println("DNS: editapp.local"); 
    } 
    server.begin(); 
    MDNS.addService("http", "tcp", 80); 
    /* initiate wecsocket */  
    initWebSocket(); 
    /* Route for root / web page */ 
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
        request->send_P(200, "text/html", index_html, processor); 
    }); 
    server.on("/robi", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send_P(200, "text/html", index_html, processor); }); 
    /* Start server */  
    server.begin(); 
} 
void loop() { 
    ws.cleanupClients(); 
    digitalWrite(ledPin, ledState); 
}
   
       
       








