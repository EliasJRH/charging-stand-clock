/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "imagepainter.h"
#include "wifihelper.h"
#include "datetimehelper.h"

DateTimeDay datetimedayinfo;

void setup() {
  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2_V2_Init();
  EPD_4IN2_V2_Clear();
  DEV_Delay_ms(500);

  //Create a new image cache
  UBYTE *BlackImage;
  
  UWORD Imagesize = ((EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1)) * EPD_4IN2_V2_HEIGHT;
  if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      printf("Failed to apply for black memory...\r\n");
      while (1);
  }
  Paint_NewImage(Canvas, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, 0, WHITE);
  Paint_SelectImage(Canvas);

  Paint_Clear(WHITE);
  wifi_connect();
  time_init();
  // wifi_disconnect();
}

void loop() {
  get_local_date_time(&datetimedayinfo);
}