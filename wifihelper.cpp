// Helper functions for WiFi

#include <stdio.h>
#include <WiFi.h>
#include "wifihelper.h"

const char* ssid     = "ssid";
const char* password = "password";

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