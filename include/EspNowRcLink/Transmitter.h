#pragma once

#include <algorithm>
#include <queue>

#include "Common.h"
#include "Protocol.h"

namespace EspNowRcLink {

class Transmitter {
public:
  enum State {
    DISCOVERING,
    TRANSMITTING,
  };

  Transmitter();
  int begin();
  int update();
  void setChannel(size_t c, unsigned int value);
  int getSensor(size_t sensorId) const;
  void commit();

private:
  template<typename M>
  void _send(const uint8_t* mac, M& m)
  {
    m.csum = checksum(m);
    WifiEspNow.send(mac, (const uint8_t*)&m, sizeof(M));
  }

  int8_t _encodeAux(int x) const;
  void _handleDiscovery();
  void _handleTransmit();
  void _handleReceived();

  static void _handleRx(const uint8_t* mac, const uint8_t *buf, size_t count, void *arg) IRAM_ATTR;
  static const uint8_t BCAST_PEER[WIFIESPNOW_ALEN];

  MessageRc _channels;
  MessageFc _sensors;
  size_t _channel = WIFI_CHANNEL_MIN;
  uint8_t _peer[WIFIESPNOW_ALEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint32_t _next_discovery = 0;
  State _state = DISCOVERING;
  std::queue<Message> _queue;
  bool _ready = false;
};

}
