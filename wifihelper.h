#ifndef _WIFIHELPER_H_
#define _WIFIHELPER_H_

void wifi_connect();
void wifi_disconnect();

String httpGETRequest(const char* host, const char* endpoint, const char* headers[]);

#endif