// Helper functions for painting images

#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "imagepainter.h"
#include <stdlib.h>

void paint_image(UBYTE *BlackImage, const unsigned char* image_name) {
  Paint_DrawBitMap(image_name);
  EPD_4IN2_V2_Display(BlackImage);
}

void paint_inspace(UBYTE *BlackImage) {
  paint_image(BlackImage, inspace_logo);
}

void paint_nasa(UBYTE *BlackImage){
  paint_image(BlackImage, epd_bitmap_nasameatball);
}