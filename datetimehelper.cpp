#include <stdint.h>
// Helper functions for getting time
#include <stdio.h>
#include <WiFi.h>
#include "time.h"
#include "datetimehelper.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * -5;
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
  datetimeinfo->datetime.Year = timeinfo.tm_year + 1900;
  datetimeinfo->datetime.Day = timeinfo.tm_mday;
  datetimeinfo->datetime.Month = timeinfo.tm_mon;
  datetimeinfo->datetime.Hour = timeinfo.tm_hour;
  datetimeinfo->datetime.Min = timeinfo.tm_min;
  datetimeinfo->datetime.Sec = timeinfo.tm_sec;
  datetimeinfo->weekday = timeinfo.tm_wday;
  
  if (USE_12_HOUR){
    if (datetimeinfo->datetime.Hour >= 0 && datetimeinfo->datetime.Hour <= 11){
      strcpy(datetimeinfo->amorpm, "AM");
    } else {
      strcpy(datetimeinfo->amorpm, "PM");
    }
    if (datetimeinfo->datetime.Hour == 0) {
      datetimeinfo->datetime.Hour = 12;
    } else if (datetimeinfo->datetime.Hour > 12) {
      datetimeinfo->datetime.Hour -= 12;
    }
  }
}

void convert_to_12hr(PAINT_TIME* time) {

}