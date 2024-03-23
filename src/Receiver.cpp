#include "EspNowRcLink/Receiver.h"
#include <limits>

namespace EspNowRcLink {

const uint8_t Receiver::BCAST_PEER[WIFIESPNOW_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void Receiver::_handleRx(const uint8_t *mac, const uint8_t *buf, size_t count, void *arg)
{
  //debugMessage(mac, buf, count);

  if(count < PAYLOAD_SIZE_MIN || count > PAYLOAD_SIZE_MAX) return;
  if(checksum(buf, count - 1) != buf[count - 2]) return;
  auto dev = reinterpret_cast<Receiver*>(arg);
  if(dev)
  {
    if(dev && buf[0] == PAIR_RES && dev->_state == BEACON)
    {
      std::copy_n(mac, WIFIESPNOW_ALEN, dev->_peer);
      dev->_state = PAIR;
    }
    if(dev && buf[0] == RC_DATA && dev->_state == RECEIVING)
    {
      std::copy_n(buf, std::min(sizeof(MessageRc), count), (uint8_t*)&dev->_channels);
      dev->_new_data = true;
    }
  }
}

Receiver::Receiver() {}

int Receiver::begin()
{
  WiFi.softAP("ESPFC-RX", nullptr, _channel, 1, 2);
  WiFi.softAPdisconnect(false);
  _wifi_set_channel(_channel);

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
      break;
  }

  return 1;
}

void Receiver::_handleBeacon()
{
  uint32_t now = millis();
  if(_next_beacon < now)
  {
    MessagePairRequest m;
    m.channel = _channel;
    _send(BCAST_PEER, m);
    _next_beacon = now + 50;
  }
}

void Receiver::_handlePair()
{
  WifiEspNow.removePeer(BCAST_PEER);
  WifiEspNow.addPeer(_peer);
  _state = RECEIVING;
}

int Receiver::available() const
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
