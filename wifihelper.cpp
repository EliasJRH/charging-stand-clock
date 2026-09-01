// Helper functions for WiFi

#include <stdio.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "wifihelper.h"

const char* ssid     = "BBFMRog";
const char* password = "LaloosHawa";

WiFiClientSecure client;

void wifi_connect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void wifi_disconnect () {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

String httpGETRequest(const char* host, const char* endpoint, const char** headers) {
  Serial.println("Connecting to network...");
  wifi_connect();
  Serial.println("Connected.");
  
  Serial.println("Sending request");
  client.setInsecure();
  if (!client.connect(host, 443)){
    Serial.println("Connection failed!");
    return "";
  } else {
    Serial.println("Connected to server, sending request");
   
    // Print raw request string to client
    client.print(String("GET ") + endpoint + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" +
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