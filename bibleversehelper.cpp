#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include "wifihelper.h"
#include "bibleversehelper.h"
#include "consts.h"

String verse_response;
const char* bible_verse_host = "api.youversion.com";
const char* verse_of_the_day_id_endpoint = "/v1/verse_of_the_days/";
const char* verse_of_the_day_passage_endpoint = "/v1/bibles/3034/passages/";
const char* key_header = "x-yvp-app-key: " YOUVERSION_API_KEY " \r\n";

void strip_invalid_chars (String *s){
  int i = 0;
  while (i < s->length()) {
    if ((int)s->charAt(i) < 32 || (int)s->charAt(i) > 126) {
      s->remove(i, 1);
    } else {
      ++i;
    }
  }
}

bool get_bible_verse(uint16_t cur_day_num, PassageInfo *passageinfo){
  uint8_t verse_id_endpoint_len = strlen(verse_of_the_day_id_endpoint) + 4;
  char endpoint_with_num[verse_id_endpoint_len];
  sprintf(endpoint_with_num, "%s%d", verse_of_the_day_id_endpoint, cur_day_num);
  
  String passage_info_payload = httpGETRequest(bible_verse_host, endpoint_with_num, key_header);
  JSONVar passage_info_json = JSON.parse(passage_info_payload);
  if (!passage_info_json.hasOwnProperty("passage_id")) return false;
  String passage_str = JSON.stringify(passage_info_json["passage_id"]);
  strncpy(passageinfo->passage_id, passage_str.substring(1, passage_str.length() - 1).c_str(), passage_str.length());

  uint8_t verse_content_endpoint_len = strlen(verse_of_the_day_passage_endpoint) + 31;
  char endpoint_with_verse[verse_content_endpoint_len];
  sprintf(endpoint_with_verse, "%s%s", verse_of_the_day_passage_endpoint, passageinfo->passage_id);

  String passage_content_payload = httpGETRequest(bible_verse_host, endpoint_with_verse, key_header);
  JSONVar passage_content_json = JSON.parse(passage_content_payload);
  if (!passage_content_json.hasOwnProperty("content") || !passage_content_json.hasOwnProperty("reference")) return false;
  String passage_content = JSON.stringify(passage_content_json["content"]);
  strip_invalid_chars(&passage_content);
  strncpy(passageinfo->content, passage_content.c_str(), passage_content.length());
  
  String passage_reference = JSON.stringify(passage_content_json["reference"]);
  strncpy(passageinfo->passage, passage_reference.c_str(), passage_reference.length());

  return true;
}

uint8_t lines_in_verse(char* verse){
  char verse_copy[500];
  memcpy(verse_copy, verse, strlen(verse));
  int passage_lines = 0;
  int running_count = 0;
  char *next_word = strtok(verse_copy, " ");
  while(next_word != NULL){
    if (running_count + strlen(next_word) + 1 <= 26) {
      running_count += strlen(next_word);
      if (running_count != 26) running_count += 1;
      next_word = strtok(NULL, " ");
    } else {
      running_count = 0;
      passage_lines++;
    }
  }
  return ++passage_lines;
}

