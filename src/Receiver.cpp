#include "EspNowRcLink/Receiver.h"
#include <limits>

namespace EspNowRcLink {

const uint8_t Receiver::BCAST_PEER[WIFIESPNOW_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void Receiver::_handleRx(const uint8_t *mac, const uint8_t *buf, size_t count, void *arg)
{
  // debugMessage(mac, buf, count);

  if(count < PAYLOAD_SIZE_MIN || count > PAYLOAD_SIZE_MAX) return;
  if(checksum(buf, count - 1) != buf[count - 1]) return;
  auto dev = reinterpret_cast<Receiver*>(arg);
  if(!dev) return;

  if(buf[0] == PAIR_RES && dev->_state == BEACON)
  {
    std::copy_n(mac, WIFIESPNOW_ALEN, dev->_peer);
    dev->_state = PAIR;
  }
  if(buf[0] == RC_DATA && dev->_state == RECEIVING && dev->_allowed(mac))
  {
    std::copy_n(buf, std::min((size_t)sizeof(MessageRc), count), (uint8_t*)&dev->_channels);
    dev->_new_data = true;
  }
}

Receiver::Receiver()
{
  _channels.ch1 = 1500;
  _channels.ch2 = 1500;
  _channels.ch3 = 1000;
  _channels.ch4 = 1500;
  _channels.ch5 = 0;
  _channels.ch6 = 0;
  _channels.ch7 = 0;
  _channels.ch8 = 0;
}

int Receiver::begin(bool enSoftAp)
{
  if(enSoftAp)
  {
    WiFi.softAP("ESPNOW-RX", nullptr, 0, 1, 2);
  }

  if(!WifiEspNow.begin()) return 0;

  if(!WifiEspNow.addPeer(BCAST_PEER)) return 0;

  WifiEspNow.onReceive(_handleRx, this);

  return 1;
}

int Receiver::update()
{
  switch(_state)
  {
    case BEACON:
      _handleBeacon();
      break;
    case PAIR:
      _handlePair();
      break;
    case RECEIVING:
      _handleAlive();
      break;
  }
  return 1;
}

void Receiver::_handleBeacon()
{
  uint32_t now = millis();
  if(now >= _next_beacon)
  {
    MessagePairRequest m;
    m.channel = WiFi.channel();
    _send(BCAST_PEER, m);
    _next_beacon = now + LINK_BEACON_INTERVAL_MS;
  }
}

void Receiver::_handlePair()
{
  WifiEspNow.removePeer(BCAST_PEER);
  WifiEspNow.addPeer(_peer);
  _state = RECEIVING;
}

void Receiver::_handleAlive()
{
  uint32_t now = millis();
  if(now >= _next_alive)
  {
    MessageAlive m;
    _send(_peer, m);
    _next_alive = now + LINK_ALIVE_INTERVAL_MS;
  }
}

int Receiver::available()
{
  bool ret = _new_data;
  _new_data = false;
  return ret;
}

uint16_t Receiver::_decodeAux(int8_t x) const
{
  return constrain(PWM_INPUT_CENTER + (x * 5), PWM_INPUT_MIN, PWM_INPUT_MAX);
}

int16_t Receiver::getChannel(int c) const
{
  switch(c)
  {
    case 0: return constrain(_channels.ch1, (int16_t)PWM_INPUT_MIN, (int16_t)PWM_INPUT_MAX);
    case 1: return constrain(_channels.ch2, (int16_t)PWM_INPUT_MIN, (int16_t)PWM_INPUT_MAX);
    case 2: return constrain(_channels.ch3, (int16_t)PWM_INPUT_MIN, (int16_t)PWM_INPUT_MAX);
    case 3: return constrain(_channels.ch4, (int16_t)PWM_INPUT_MIN, (int16_t)PWM_INPUT_MAX);
    case 4: return _decodeAux(_channels.ch5);
    case 5: return _decodeAux(_channels.ch6);
    case 6: return _decodeAux(_channels.ch7);
    case 7: return _decodeAux(_channels.ch8);
  }
  return 0;
}

bool Receiver::_allowed(const uint8_t *mac) const
{
  return std::equal(_peer, _peer + WIFIESPNOW_ALEN, mac);
}

template<typename T>
T sanitizeSensor(int v)
{
  if(v > std::numeric_limits<T>::max()) return std::numeric_limits<T>::max();
  if(v < std::numeric_limits<T>::min()) return std::numeric_limits<T>::min();
  return static_cast<T>(v);
}

int Receiver::setSensor(int sensorId, int sensorValue)
{
  switch(sensorId)
  {
    case 0: _sensors.ch1 = sanitizeSensor<int16_t>(sensorValue); break;
    case 1: _sensors.ch2 = sanitizeSensor<int16_t>(sensorValue); break;
    case 2: _sensors.ch3 = sanitizeSensor<int8_t>(sensorValue); break;
    case 3: _sensors.ch4 = sanitizeSensor<int8_t>(sensorValue); break;
  }
  return 0;
}

}
