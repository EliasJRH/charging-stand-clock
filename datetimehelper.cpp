// Helper functions for getting time
#include <stdio.h>
#include <WiFi.h>
#include "time.h"
#include "datetimehelper.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void time_init() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void get_local_date_time(DateTimeDay* datetimeinfo) {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    printf("Failed to obtain time\n");
    return;
  }
  datetimeinfo->datetime.Year = timeinfo.tm_year;
  datetimeinfo->datetime.Month = timeinfo.tm_mon;
}