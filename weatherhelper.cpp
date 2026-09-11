#include <stdio.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include "weatherhelper.h"
#include "wifihelper.h"
#include "consts.h"

const char* local_weather_host = "api.open-meteo.com";
const char* local_weather_endpoint = "/v1/forecast?latitude=" LAT "&longitude=" LONG "&daily=temperature_2m_max,temperature_2m_min&current=temperature_2m,apparent_temperature,weather_code&timezone=auto&forecast_days=1";

char* get_desc_from_code(int weather_code){
  switch(weather_code){
    case 0:
      return "Clear skies";
      break;
    case 1:
      return "Mainly clear";
      break;
    case 2:
      return "Partly cloudy";
      break;
    case 3:
      return "Overcast";
      break;
    case 45:
    case 48:
      return "Foggy";
      break;
    case 51:
      return "Light drizzle";
      break;
    case 53:
    case 55:
      return "Moderate drizzle";
      break;
    case 56:
      return "Light freezing drizzle";
      break;
    case 57:
      return "Moderate freezing drizzle";
      break;
    case 61:
      return "Slight rain";
      break;
    case 63:
      return "Moderate rain";
      break;
    case 65:
      return "Heavy rain";
      break;
    case 66:
      return "Light freezing rain";
      break;
    case 67:
      return "Heavy freezing rain";
      break;
    case 71:
      return "Slight snow";
      break;
    case 73:
      return "Moderate snow";
      break;
    case 75:
    case 77:
      return "Heavy snow";
      break;
    case 80:
      return "Slight rain showers";
      break;
    case 81:
      return "Moderate rain showers";
      break;
    case 82:
      return "Heavy rain showers";
      break;
    case 85:
      return "Slight snow showers";
      break;
    case 86:
      return "Heavy snow showers";
      break;
    case 95:
      return "Slight thunderstorm";
      break;
    case 96:
      return "Thunderstorm with slight rain";
      break;
    case 99:
      return "Thunderstorm with heavy rain";
      break;
    default:
      return "";
      break;
  }
}

bool update_weather(Weather* weather){
  String payload = httpGETRequest(local_weather_host, local_weather_endpoint, "");
  if (payload.length() == 0) return false;

  JSONVar payload_json = JSON.parse(payload);

  if (payload_json.length() == 0) return false;
  if (!payload_json.hasOwnProperty("current")) return false;
  if (!payload_json.hasOwnProperty("daily")) return false;

  JSONVar temp = payload_json["current"];
  if (!temp.hasOwnProperty("temperature_2m") || !temp.hasOwnProperty("apparent_temperature") || !temp.hasOwnProperty("weather_code")) return false;
  temp = payload_json["daily"];
  if (!temp.hasOwnProperty("temperature_2m_min") || !temp.hasOwnProperty("temperature_2m_max")) return false;

  weather->temp = int(round(double(payload_json["current"]["temperature_2m"])));
  weather->feels_like = int(round(double(payload_json["current"]["apparent_temperature"])));
  char* desc = get_desc_from_code(payload_json["current"]["weather_code"]);
  memset(weather->desc, 0, sizeof(weather->desc));
  memcpy(weather->desc, desc, strlen(desc));
  weather->min = int(round(double(payload_json["daily"]["temperature_2m_min"][0])));
  weather->max = int(round(double(payload_json["daily"]["temperature_2m_max"][0])));
  return true;
}