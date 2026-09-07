// Helper functions for WiFi

#include <stdio.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "wifihelper.h"
#include "consts.h"

const char* ssid     = SSID;
const char* password = PASSWORD;

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

String httpGETRequest(const char* host, const char* endpoint, const char* headers) {
  wifi_connect();
  
  Serial.print("Sending request to ");
  Serial.print(host);
  Serial.println(endpoint);
  client.setInsecure();
  if (!client.connect(host, 443)){
    Serial.println("Connection failed!");
    return "";
  } else {
    Serial.println("Connected to server, sending request");

    Serial.print("Header sent: ");
    Serial.println(headers);
   
    // Print raw request string to client
    client.print(String("GET ") + endpoint + " HTTP/1.0\r\n" +
               "Host: " + host + "\r\n" + headers +
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