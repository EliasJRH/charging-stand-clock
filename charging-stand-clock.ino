#include <stdlib.h>
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "imagepainter.h"
#include "wifihelper.h"
#include "datetimehelper.h"
#include "weatherhelper.h"

DateTimeDay datetimedayinfo;
Weather weather;
uint8_t last_hour = 25;
//Create a new image cache
UBYTE *Canvas;
char date_buf[30];
char temp_buf[5];
char feels_like_buf[20];
char min_max_buf[20];


void setup() {
  DEV_Module_Init();
  EPD_4IN2_V2_Init();
  EPD_4IN2_V2_Clear();
  DEV_Delay_ms(500);
  Serial.begin(9600);

  UWORD CanvasSize = ((EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1)) * EPD_4IN2_V2_HEIGHT;
  if((Canvas = (UBYTE *)malloc(CanvasSize)) == NULL) {
      printf("Failed to allocate memory...\r\n");
      while (1);
  }
  Paint_NewImage(Canvas, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, 0, WHITE);
  Paint_SelectImage(Canvas);

  Paint_Clear(WHITE);
  wifi_connect();
  time_init();
  // wifi_disconnect();
  EPD_4IN2_V2_Display(Canvas);
  Paint_SetRotate(ROTATE_270);
}

void loop() {
  get_local_date_time(&datetimedayinfo);
  Paint_Clear(WHITE);

  // Date and time
  memset(date_buf, 0, sizeof date_buf);
  sprintf(date_buf, "%s %s %u %u", day_names[datetimedayinfo.weekday], month_names[datetimedayinfo.datetime.Month], datetimedayinfo.datetime.Day + 1, datetimedayinfo.datetime.Year);
  Paint_DrawTime(25, 20, &(datetimedayinfo.datetime), &FontCascadia3, WHITE, BLACK);
  Paint_DrawString_EN(255, 50, datetimedayinfo.amorpm, &Font20, WHITE, BLACK);
  Paint_DrawString_EN(52, 80, date_buf, &Font20, WHITE, BLACK);
  
  // Weather 
  if (last_hour != datetimedayinfo.datetime.Hour) {
    last_hour = datetimedayinfo.datetime.Hour;
    while(!update_weather(&weather)){
      delay(500);
    };
  }
  int offset = abs(weather.temp) < 10 ? floor(FontCascadia3.Width/2) : 0;
  Paint_DrawNum(90 + offset, 120, weather.temp, &FontCascadia3, BLACK, WHITE);
  Paint_DrawCircle(170 - offset, 135, 4, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawChar(180 - offset, 120, 'C', &FontCascadia3, BLACK, WHITE);

  memset(feels_like_buf, 0, sizeof feels_like_buf);
  sprintf(feels_like_buf, "Feels like: %d", weather.feels_like);
  Paint_DrawString_EN(55, 180, feels_like_buf, &Font20, WHITE, BLACK);

  memset(min_max_buf, 0, sizeof min_max_buf);
  sprintf(min_max_buf, "High: %d | Low: %d", weather.max, weather.min);
  Paint_DrawString_EN(25, 200, min_max_buf, &Font20, WHITE, BLACK);

  EPD_4IN2_V2_PartialDisplay(Canvas, 0, 0, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT);
  DEV_Delay_ms(250);
}