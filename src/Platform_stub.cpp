#if !defined(ARDUINO_ARCH_ESP32) && !defined(ARDUINO_ARCH_ESP8266)

#include "Platform.h"
#include <chrono>

namespace EspNowRcLink {
namespace Platform {

uint32_t millis()
{
  using namespace std::chrono;
  static const auto start = steady_clock::now();
  return static_cast<uint32_t>(duration_cast<milliseconds>(steady_clock::now() - start).count());
}

bool espnowBegin() { return false; }
void espnowEnd() {}
bool addPeer(const uint8_t *) { return false; }
bool removePeer(const uint8_t *) { return false; }
bool send(const uint8_t *, const uint8_t *, size_t) { return false; }
void onReceive(RxCallback, void *) {}

void setWifiChannel(uint8_t) {}
uint8_t getWifiChannel() { return 0; }

bool softApBegin(const char *, uint8_t) { return false; }
void softApEnd() {}

void debugMessage(const uint8_t *, const uint8_t *, size_t) {}

}
}

#endif
