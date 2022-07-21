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

  <div id='div1' style='background-color:#00A0A0; width:auto; height:auto; font-size:20px; font-weight:bold; text-align:center; color:#FFFFFF;'>Robot Car
  </div>
  <div id='div2' style='text-align:center;'>
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
     document.getElementById("div2").innerHTML="Control the Robot Car";
   </script> 
   
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
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
   }
   window.addEventListener('load', onLoad);
   function onLoad(event) {
    initWebSocket();
    initButton();
   }
   function initButton() {
    document.getElementById('button').addEventListener('click', toggle); 
    document.getElementById('forward').addEventListener('click', forward); 
    document.getElementById('backward').addEventListener('click', backward); 
    document.getElementById('left').addEventListener('click', left); 
    document.getElementById('right').addEventListener('click', right); 
    document.getElementById('stop').addEventListener('click', stop); 
   }
   function toggle(){
    websocket.send('toggle');
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
   
       
       








