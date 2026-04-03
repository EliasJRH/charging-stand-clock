#ifndef _WEATHERHELPER_H_
#define _WEATHERHELPER_H_

typedef struct {
  uint8_t temp;
  uint8_t feels_like;
  uint8_t min;
  uint8_t max;
  char* desc;
} Weather;

void update_weather(Weather* weather);

#endif