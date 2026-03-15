#ifndef _DATETIMEHELPER_H_
#define _DATETIMEHELPER_H_

#include "GUI_Paint.h"

typedef struct {
  PAINT_TIME datetime;
  int weekday;
} DateTimeDay;

void time_init();
void get_local_date_time(DateTimeDay* datetimeinfo);

#endif