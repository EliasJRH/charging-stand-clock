#include <stdio.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include "weatherhelper.h"
#include "wifihelper.h"

WiFiClientSecure client;
String weather_response;
const char* weather_host = "api.openweathermap.org";
const char* curr_weather_path = "/data/2.5/weather?lat=45.41117&lon=-75.69812&appid=<key>0&units=metric";

String httpGETRequest(const char* endpoint) {
  // attempt to connect to Wifi network:
  Serial.println("Connecting to network...");
  wifi_connect();
  Serial.println("Connected.");
  
  Serial.println("Sending request");
  client.setInsecure();
  if (!client.connect(weather_host, 443)){
    Serial.println("Connection failed!");
    return "";
  } else {
    Serial.println("Connected to server, sending request");
   
    // Make request to endpoint:
    client.print(String("GET ") + endpoint + " HTTP/1.1\r\n" +
               "Host: " + weather_host + "\r\n" +
               "Connection: close\r\n\r\n");

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    Serial.println("Output:");
    String payload = "";
    while (client.available()) {
      char c = client.read();
      payload += c;
      Serial.write(c);
    }

    client.stop();
    return "";
  }
}

void update_weather(Weather* weather){
  httpGETRequest(curr_weather_path);
}