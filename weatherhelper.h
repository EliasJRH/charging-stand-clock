#ifndef _WEATHERHELPER_H_
#define _WEATHERHELPER_H_

typedef struct {
  float temp;
  float feels_like;
  float min;
  float max;
  char* desc;
} Weather;

void update_weather(Weather* weather);

#endif