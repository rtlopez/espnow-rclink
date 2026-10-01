#pragma once

#include <cstddef>
#include <cstdint>

namespace EspNowRcLink {
namespace Platform {

using RxCallback = void (*)(const uint8_t *mac, const uint8_t *buf, size_t count, void *arg);

uint32_t millis();

bool espnowBegin();
void espnowEnd();
bool addPeer(const uint8_t *mac);
bool removePeer(const uint8_t *mac);
bool send(const uint8_t *mac, const uint8_t *buf, size_t count);
void onReceive(RxCallback cb, void *arg);

void setWifiChannel(uint8_t channel);
uint8_t getWifiChannel();

bool softApBegin(const char *ssid, uint8_t channel);
void softApEnd();

void debugMessage(const uint8_t *mac, const uint8_t *buf, size_t count);

}
}
