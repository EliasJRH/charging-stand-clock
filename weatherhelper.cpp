#include <stdio.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include "weatherhelper.h"
#include "wifihelper.h"

const char* weather_host = "api.open-meteo.com";
const char* local_weather_endpoint = "/v1/forecast?latitude=lat&longitude=lon&daily=temperature_2m_max,temperature_2m_min&models=gem_seamless&current=temperature_2m,apparent_temperature&timezone=auto&forecast_days=1";

bool update_weather(Weather* weather){
  String payload = httpGETRequest(weather_host, local_weather_endpoint, "");
  if (payload.length() == 0) return false;

  JSONVar payload_json = JSON.parse(payload);

  if (payload_json.length() == 0) return false;
  if (!payload_json.hasOwnProperty("current")) return false;
  if (!payload_json.hasOwnProperty("daily")) return false;

  JSONVar temp = payload_json["current"];
  if (!temp.hasOwnProperty("temperature_2m") || !temp.hasOwnProperty("apparent_temperature")) return false;
  temp = payload_json["daily"];
  if (!temp.hasOwnProperty("temperature_2m_min") || !temp.hasOwnProperty("temperature_2m_max")) return false;

  weather->temp = int(round(double(payload_json["current"]["temperature_2m"])));
  weather->feels_like = int(round(double(payload_json["current"]["apparent_temperature"])));
  weather->min = int(round(double(payload_json["daily"]["temperature_2m_min"][0])));
  weather->max = int(round(double(payload_json["daily"]["temperature_2m_max"][0])));
  return true;
}