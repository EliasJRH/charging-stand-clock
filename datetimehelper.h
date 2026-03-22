#ifndef _DATETIMEHELPER_H_
#define _DATETIMEHELPER_H_

#define USE_12_HOUR true

#include "GUI_Paint.h"

static const char* day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char* month_names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dev"};

typedef struct {
  PAINT_TIME datetime;
  int weekday;
  char amorpm[3];
} DateTimeDay;

void time_init();
void get_local_date_time(DateTimeDay* datetimeinfo);

#endif