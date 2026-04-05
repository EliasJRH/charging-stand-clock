#ifndef _WEATHERHELPER_H_
#define _WEATHERHELPER_H_

typedef struct {
  int16_t temp;
  int16_t feels_like;
  int16_t min;
  int16_t max;
  char* desc;
} Weather;

bool update_weather(Weather* weather);

#endif