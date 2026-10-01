#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)

#include "Platform.h"
#include <Arduino.h>
#include <WifiEspNow.h>

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <user_interface.h>
#else
#include <WiFi.h>
#include <esp_wifi.h>
#endif

namespace EspNowRcLink {
namespace Platform {

uint32_t millis()
{
  return ::millis();
}

bool espnowBegin()
{
  return WifiEspNow.begin();
}

void espnowEnd()
{
  WifiEspNow.end();
}

bool addPeer(const uint8_t *mac)
{
  return WifiEspNow.addPeer(mac);
}

bool removePeer(const uint8_t *mac)
{
  return WifiEspNow.removePeer(mac);
}

bool send(const uint8_t *mac, const uint8_t *buf, size_t count)
{
  return WifiEspNow.send(mac, buf, count);
}

void onReceive(RxCallback cb, void *arg)
{
  WifiEspNow.onReceive(cb, arg);
}

void setWifiChannel(uint8_t channel)
{
#if defined(ARDUINO_ARCH_ESP8266)
  wifi_set_channel(channel);
#else
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
#endif
}

uint8_t getWifiChannel()
{
  return WiFi.channel();
}

bool softApBegin(const char *ssid, uint8_t channel)
{
  return WiFi.softAP(ssid, nullptr, channel, 1);
}

void softApEnd()
{
  WiFi.softAPdisconnect(true);
}

void debugMessage(const uint8_t *mac, const uint8_t *buf, size_t count)
{
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X> ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  for (size_t i = 0; i < count; ++i)
  {
    Serial.printf("%02X ", buf[i]);
  }
  Serial.println();
}

}
}

#endif
