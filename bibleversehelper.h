#ifndef _BIBLEVERSEHELPER_H_
#define _BIBLEVERSEHELPER_H_

#include <stdint.h>

typedef struct {
  char passage_id[30];
  char passage[30];
  char content[500];
} PassageInfo;

bool get_bible_verse(uint16_t cur_day_num, PassageInfo *passageinfo);
uint8_t lines_in_verse(char* verse);

#endif