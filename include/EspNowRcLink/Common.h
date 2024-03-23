#pragma once

#include <WifiEspNow.h>

#if defined(ARDUINO_ARCH_ESP8266)

#include <ESP8266WiFi.h>
#include <user_interface.h>

#define _wifi_set_channel(c) wifi_set_channel(c)

#elif defined(ARDUINO_ARCH_ESP32)

#include <WiFi.h>
#include <esp_wifi.h>

#define _wifi_set_channel(c) esp_wifi_set_channel(c, WIFI_SECOND_CHAN_NONE)

#endif

#include "Protocol.h"

namespace EspNowRcLink {

struct Message
{
  uint8_t mac[WIFIESPNOW_ALEN];
  uint8_t len;
  union {
    uint8_t type;
    uint8_t payload[PAYLOAD_SIZE_MAX];
  };
};

inline uint8_t checksum(const uint8_t *data, size_t len)
{
  uint8_t csum = 0x55;
  for(size_t i = 0; i < len; i++)
  {
    csum ^= data[i];
  }
  return csum;
}

template<typename M>
uint8_t checksum(const M& m)
{
  const uint8_t *data = reinterpret_cast<const uint8_t*>(&m);
  const size_t len = sizeof(M) - 1;
  return checksum(data, len);
}

void inline debugMessage(const uint8_t *mac, const uint8_t *buf, size_t count)
{
  Serial.printf("[%02X:%02X:%02X:%02X:%02X:%02X]> ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  for (size_t i = 0; i < count; ++i)
  {
    Serial.print(buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

}
