#include <ESP8266WiFi.h>
#include <WiFiClient.h>
const char* ssid = "DepotLab-Guest";
const char* password = "depotlabguest20";
 
const int sensor = 1;
const int sensorOUT = 2;
long duration, cm, inches;
 
const char* host = "maker.ifttt.com";
const int httpsPort = 80;
String key = "8GrwCSdPqlkFLYylixP6P";
String evento = "post_tweet";

WiFiClient client;
void setup() {
 pinMode(sensorOUT,OUTPUT);
 pinMode(sensor,INPUT);
 Serial.begin(115200);
 Serial.println();
 Serial.print("Conectando wifi: ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi conectado!");
 Serial.println("IP: ");
 Serial.println(WiFi.localIP());
 
}
 
void loop() {
  
  digitalWrite(sensorOUT, LOW);
  delayMicroseconds(5);
  digitalWrite(sensorOUT, HIGH);
  delayMicroseconds(1000);
  digitalWrite(sensorOUT, LOW);
  duration = pulseIn(sensor, HIGH);
    cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println(); 
  
  if(digitalRead(sensor))
 {
  Serial.println("Sensor activado");
  Serial.print("Conectando a: ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
   Serial.println("conexion fallida");
   return;
 }
 
 String url = "/trigger/"+evento+"/with/key/"+key;
 
 Serial.print("requesting URL: ");
 Serial.println(url);
 
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
 "Host: " + host + "\r\n" +
 "User-Agent: Arduino\r\n" +
 "Connection: close\r\n\r\n");
 
 Serial.println("request sent");
 while (client.connected()) {
  String line = client.readStringUntil('\n');
  if (line == "\r") {
   Serial.println("headers received");
   break;
  }
 }
 String line = client.readStringUntil('\n');
 
 Serial.println("reply was:");
 Serial.println("==========");
 Serial.println(line);
 Serial.println("==========");
 Serial.println("closing connection");
 
 delay(10000);
 }
}
