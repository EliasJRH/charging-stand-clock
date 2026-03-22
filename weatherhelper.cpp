#include <stdio.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "weatherhelper.h"

WiFiClient client;
HTTPClient https;
String weather_response;
const char* curr_weather_api = "https://api.openweathermap.org/data/2.5/weather?lat=45.41117&lon=-75.69812&appid=61428bdd12c68e07a373134164b7de36&units=metric";
const char* weather_api_cert = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIG5jCCBc6gAwIBAgIQT0UaOwW0bnvATrvIc88vkDANBgkqhkiG9w0BAQsFADCB\n" \
  "lTELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4G\n" \
  "A1UEBxMHU2FsZm9yZDEYMBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMT0wOwYDVQQD\n" \
  "EzRTZWN0aWdvIFJTQSBPcmdhbml6YXRpb24gVmFsaWRhdGlvbiBTZWN1cmUgU2Vy\n" \
  "dmVyIENBMB4XDTI1MDMxMDAwMDAwMFoXDTI2MDQxMDIzNTk1OVowWDELMAkGA1UE\n" \
  "BhMCR0IxDzANBgNVBAgTBkxvbmRvbjEZMBcGA1UEChMQT3BlbndlYXRoZXIgTHRk\n" \
  "LjEdMBsGA1UEAwwUKi5vcGVud2VhdGhlcm1hcC5vcmcwggEiMA0GCSqGSIb3DQEB\n" \
  "AQUAA4IBDwAwggEKAoIBAQDez9ygRdpOOVi6hV8iixWWqex/6XWQz4vQMT9ubDLG\n" \
  "bCjETHhVSgZcM7P2zfKp6hCA95+9GGBsCQBZoeVXpYIqd3xyL+c+eImtS5rdUgCu\n" \
  "nKQ7xLJKZeP4Dp/oIrK722sVbU06Rt6EyB9uTWs+Da/Jtqv/Klg2DvH3jooaYudZ\n" \
  "WuEPFG6efH8GB3IWEHL7199b7uq1w72/344gqJuIoSfVsRp8RTh3k79UIIKm1ira\n" \
  "XYwWSPRFU840HcdJZbqDfmxDxzdFCKlepUZ5w3WpYFpWq8F5SbiEepllY80gTugO\n" \
  "vvAEuJjRNsRR0tLzrP7op/Gvbutqp+BDEm4dGtvBcKHrAgMBAAGjggNsMIIDaDAf\n" \
  "BgNVHSMEGDAWgBQX2dYlJ2f5McJJQ9kwNkSMbKlP6zAdBgNVHQ4EFgQUS2G3/XaE\n" \
  "XKrnDtu7hjMJz2oRokowDgYDVR0PAQH/BAQDAgWgMAwGA1UdEwEB/wQCMAAwHQYD\n" \
  "VR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMEoGA1UdIARDMEEwNQYMKwYBBAGy\n" \
  "MQECAQMEMCUwIwYIKwYBBQUHAgEWF2h0dHBzOi8vc2VjdGlnby5jb20vQ1BTMAgG\n" \
  "BmeBDAECAjBaBgNVHR8EUzBRME+gTaBLhklodHRwOi8vY3JsLnNlY3RpZ28uY29t\n" \
  "L1NlY3RpZ29SU0FPcmdhbml6YXRpb25WYWxpZGF0aW9uU2VjdXJlU2VydmVyQ0Eu\n" \
  "Y3JsMIGKBggrBgEFBQcBAQR+MHwwVQYIKwYBBQUHMAKGSWh0dHA6Ly9jcnQuc2Vj\n" \
  "dGlnby5jb20vU2VjdGlnb1JTQU9yZ2FuaXphdGlvblZhbGlkYXRpb25TZWN1cmVT\n" \
  "ZXJ2ZXJDQS5jcnQwIwYIKwYBBQUHMAGGF2h0dHA6Ly9vY3NwLnNlY3RpZ28uY29t\n" \
  "MDMGA1UdEQQsMCqCFCoub3BlbndlYXRoZXJtYXAub3JnghJvcGVud2VhdGhlcm1h\n" \
  "cC5vcmcwggF9BgorBgEEAdZ5AgQCBIIBbQSCAWkBZwB1AJaXZL9VWJet90OHaDcI\n" \
  "Qnfp8DrV9qTzNm5GpD8PyqnGAAABlYD2g6UAAAQDAEYwRAIgbOk7De/23Y+kAcIz\n" \
  "0UQeR8CnzxY4XvloLvOWpbvK7XYCIFQ5L18pKmCgUm8dSKhszz34SHspwx3M++dw\n" \
  "+r2OKI/fAHYAGYbUxyiqb/66A294Kk0BkarOLXIxD67OXXBBLSVMx9QAAAGVgPaD\n" \
  "ggAABAMARzBFAiBkfIaw27H3oE4ywuhdv6VMG1/mT9U0v+T+0xcRCCTJHAIhAI0y\n" \
  "L1YA4cSQ/FYUSpSvUloNI9xz0IMkmhHnNGgz6XSqAHYAyzj3FYl8hKFEX1vB3fvJ\n" \
  "bvKaWc1HCmkFhbDLFMMUWOcAAAGVgPaDrAAABAMARzBFAiEA7+0Lopd0v6NPstlB\n" \
  "HHbGl3/QRlbYKRivzoafxNCOUCMCIB834fDX2cTykcFaGkhJtRIHNqvaqKdpQ2M6\n" \
  "vTNf6wczMA0GCSqGSIb3DQEBCwUAA4IBAQCWy6Rd4qLxQaf9qjksvrir1z43PTu0\n" \
  "ye8tKXUFiIhL9UKhgXODNltW1B3eUcGOxv49bxPgiRNOBsVdsW4wVRM54Wm2WkzU\n" \
  "PMaPVeSY2CTv3UE2lxW1yil9xfIvgibrNkig5DDIN62LgrqeKY/P/g1d43EQ2PCr\n" \
  "gjZLvs/XAGUxhsfXJPvw/eeOMtvym5YBgHJ69gmVlDerK9aIT9xxl0fxINaNs02E\n" \
  "jJzzRaJfVJoRGPTz6fy1Gsf6nTMI+RutiSxteZHwXXLpeqUKvJnX23/QfDQ+lzYJ\n" \
  "hrhm8OajSkgLZclOz2Tv4ldw9atzH/jcjcWbHzAtrAnasRbh5O9aEiw2\n" \
  "-----END CERTIFICATE-----\n";


String httpGETRequest(const char* endpoint) {
}

void update_weather(Weather* weather){
}