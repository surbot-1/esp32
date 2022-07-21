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










