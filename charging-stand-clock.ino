#include <stdlib.h>
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "imagepainter.h"
#include "wifihelper.h"
#include "datetimehelper.h"
#include "weatherhelper.h"
#include "bibleversehelper.h"

DateTimeDay datetimedayinfo;
Weather weather;
PassageInfo passageinfo;
uint8_t last_hour = 25;
uint16_t cur_day_num = 400;
//Create a new image cache
UBYTE *Canvas;
char date_buf[30];
char temp_buf[5];
char feels_like_buf[20];
char min_max_buf[20];
char full_passage_buf[500];
char passage_line_buf[35];
char passage_ref_buf[25];
uint8_t days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
uint16_t days = 0;
const uint8_t MAX_VERSE_LINES = 8;

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
  Paint_NewImage(Canvas, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, ROTATE_270, WHITE);
  Paint_SelectImage(Canvas);

  Paint_Clear(WHITE);
  wifi_connect();
  time_init();
  EPD_4IN2_V2_Display(Canvas);
}

void loop() {
  get_local_date_time(&datetimedayinfo);
  Paint_Clear(WHITE);

  // Date and time
  memset(date_buf, 0, sizeof date_buf);
  sprintf(date_buf, "%s %s %u %u", day_names[datetimedayinfo.weekday], month_names[datetimedayinfo.datetime.Month], datetimedayinfo.datetime.Day + 1, datetimedayinfo.datetime.Year);
  Paint_DrawTime(25, 20, &(datetimedayinfo.datetime), &FontCascadia3, WHITE, BLACK);
  Paint_DrawString_EN(255, 50, datetimedayinfo.amorpm, &Font20, WHITE, BLACK);
  Paint_DrawString_EN(floor((EPD_4IN2_V2_HEIGHT - strlen(date_buf) * Font20.Width) / 2), 80, date_buf, &Font20, WHITE, BLACK);
  
  // Weather 
  if (last_hour != datetimedayinfo.datetime.Hour) {
    last_hour = datetimedayinfo.datetime.Hour;
    while(!update_weather(&weather)){
      delay(2000);
    };
  }

  // Get day no. for bible verse
  days = 0;
  for(uint8_t i = 0; i < datetimedayinfo.datetime.Month; ++i){
    days += days_in_month[i];
  }
  days += datetimedayinfo.datetime.Day;
  if (cur_day_num != days){
    cur_day_num = days;
    get_bible_verse(cur_day_num, &passageinfo);
  }

  int offset = abs(weather.temp) < 10 ? floor(FontCascadia3.Width/2) : 0;
  offset -= weather.temp < 0 ? floor(FontCascadia3.Width/2) : 0;
  Paint_DrawNum(90 + offset, 120, weather.temp, &FontCascadia3, BLACK, WHITE);
  Paint_DrawCircle(170 - offset, 135, 4, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawChar(180 - offset, 120, 'C', &FontCascadia3, BLACK, WHITE);

  memset(feels_like_buf, 0, sizeof feels_like_buf);
  sprintf(feels_like_buf, "Feels like: %d", weather.feels_like);
  Paint_DrawString_EN(floor((EPD_4IN2_V2_HEIGHT - strlen(feels_like_buf) * Font20.Width) / 2), 180, feels_like_buf, &Font20, WHITE, BLACK);

  memset(min_max_buf, 0, sizeof min_max_buf);
  sprintf(min_max_buf, "High: %d | Low: %d", weather.max, weather.min);
  Paint_DrawString_EN(floor((EPD_4IN2_V2_HEIGHT - strlen(min_max_buf) * Font20.Width) / 2), 200, min_max_buf, &Font20, WHITE, BLACK);

  Paint_DrawString_EN(floor((EPD_4IN2_V2_HEIGHT - strlen(weather.desc) * Font20.Width) / 2), 220, weather.desc, &Font20, WHITE, BLACK);

  int passage_line = 0;
  memset(full_passage_buf, 0, sizeof(full_passage_buf));
  memcpy(full_passage_buf, passageinfo.content, strlen(passageinfo.content));
  memset(passage_line_buf, 0, sizeof(passage_line_buf));

  int passage_lines = lines_in_verse(full_passage_buf);
  int passage_y_center_offset = 230 + floor((170 - ((passage_lines >= MAX_VERSE_LINES ? MAX_VERSE_LINES : passage_lines + 1) * Font16.Height)) / 2);
  char *next_word = strtok(full_passage_buf, " ");
  while(next_word != NULL){
    if (passage_line == MAX_VERSE_LINES) break;

    if (strlen(passage_line_buf) + strlen(next_word) + 1 <= 26){
      strcat(passage_line_buf, next_word);
      if (strlen(passage_line_buf) != 26) strcat(passage_line_buf, " ");
      next_word = strtok(NULL, " ");
    } else {
      if (passage_line == 6 && strlen(passage_line_buf) + 4 < 26) strcat(passage_line_buf, "...");
      Paint_DrawString_EN(16, passage_y_center_offset + (Font16.Height * (passage_line++)), passage_line_buf, &Font16, WHITE, BLACK);
      memset(passage_line_buf, 0, sizeof(passage_line_buf));
    }
  }

  if (passage_line != MAX_VERSE_LINES) Paint_DrawString_EN(16, passage_y_center_offset + (Font16.Height * (passage_line++)), passage_line_buf, &Font16, WHITE, BLACK);
  sprintf(passage_ref_buf, "- %s", passageinfo.passage);
  Paint_DrawString_EN(285 - (Font16.Width * strlen(passage_ref_buf)), passage_y_center_offset + (Font16.Height * (passage_line)), passage_ref_buf, &Font16, WHITE, BLACK);

  EPD_4IN2_V2_PartialDisplay(Canvas, 0, 0, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT);
  DEV_Delay_ms(250);
}