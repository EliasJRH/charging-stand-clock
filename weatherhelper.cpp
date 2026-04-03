#include <stdio.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include "weatherhelper.h"
#include "wifihelper.h"

WiFiClientSecure client;
String weather_response;
const char* weather_host = "api.open-meteo.com";
const char* curr_weather_path = "/v1/forecast?latitude=lat&longitude=-lon&daily=temperature_2m_max,temperature_2m_min&models=gem_seamless&current=temperature_2m,apparent_temperature&timezone=auto&forecast_days=1";

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
   
    // Print raw request string to client
    client.print(String("GET ") + endpoint + " HTTP/1.0\r\n" +
               "Host: " + weather_host + "\r\n" +
               "Connection: close\r\n\r\n");

    // Ignore headers
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    // Save output to payload
    String payload = "";
    while (client.available()) {
      char c = client.read();
      payload += c;
    }

    client.stop();
    return payload;
  }
}

void update_weather(Weather* weather){
  String payload = httpGETRequest(curr_weather_path);
  if (payload.length() == 0) return;

  JSONVar payload_json = JSON.parse(payload);
  weather->temp = int(round(double(payload_json["current"]["temperature_2m"])));
  weather->feels_like = int(round(double(payload_json["current"]["apparent_temperature"])));
  weather->min = int(round(double(payload_json["daily"]["temperature_2m_min"][0])));
  weather->max = int(round(double(payload_json["daily"]["temperature_2m_max"][0])));
}