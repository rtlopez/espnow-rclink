#include "EspNowRcLink/Transmitter.h"

namespace EspNowRcLink {

const uint8_t Transmitter::BCAST_PEER[WIFIESPNOW_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void Transmitter::_handleRx(const uint8_t *mac, const uint8_t *buf, size_t count, void *arg)
{
  //debugMessage(mac, buf, count);

  if(count < PAYLOAD_SIZE_MIN || count > PAYLOAD_SIZE_MAX) return;
  if(checksum(buf, count - 1) != buf[count - 2]) return;
  auto dev = reinterpret_cast<Transmitter*>(arg);
  if(dev)
  {
    Message m;
    std::copy_n(mac, WIFIESPNOW_ALEN, m.mac);
    m.len = std::min(count, PAYLOAD_SIZE_MAX);
    std::copy_n(buf, m.len, m.payload);
    dev->_queue.push(m);
  }
}

Transmitter::Transmitter(): _channel(WIFI_CHANNEL_DEFAULT) {}

int Transmitter::begin()
{
  if(!WiFi.softAP("ESPNOW-TX", nullptr, _channel, 1, 2)) return 0;

  if (!WifiEspNow.begin()) return 0;

  WifiEspNow.onReceive(_handleRx, this);

  return 1;
}

int Transmitter::update()
{
  switch(_state)
  {
    case DISCOVERING:
      // scan wifi channels
      _handleDiscovery();
      break;
    case TRANSMITTING:
      // send rc channels
      _handleTransmit();
  }

  // process response queue
  _handleReceived();

  return 0;
}

void Transmitter::_handleDiscovery()
{
  // increment channel every 200mc
  uint32_t now = millis();
  if(_next_discovery < now)
  {
    _channel++;
    if(_channel > WIFI_CHANNEL_MAX) _channel = WIFI_CHANNEL_MIN;
    _wifi_set_channel(_channel);
    _next_discovery = now + 200;
  }
}

void Transmitter::_handleTransmit()
{
  if(_ready)
  {
    _send(_peer, _channels);
    _ready = false;
  }
}

void Transmitter::_handleReceived()
{
  for (; !_queue.empty(); _queue.pop())
  {
    const Message& m = _queue.front();

    //debugMessage(m.mac, m.payload, m.len);

    switch(m.type)
    {
      case PAIR_REQ:
      {
        const auto pr = reinterpret_cast<const MessagePairRequest*>(m.payload);
        if(_state == DISCOVERING && pr->channel >= WIFI_CHANNEL_MIN && pr->channel <= WIFI_CHANNEL_MAX)
        {
          _channel = pr->channel; // assign channel
          _wifi_set_channel(_channel);
          _state = TRANSMITTING; // stop discovery

          std::copy_n(m.mac, WIFIESPNOW_ALEN, _peer); // remember peer address
          WifiEspNow.addPeer(_peer);
          MessagePairResponse m;
          _send(_peer, m); // notify receiver
        }
        break;
      }
      case FC_DATA:
        std::copy_n(m.payload, sizeof(MessageFc), (uint8_t*)&_sensors);
        break;

      default:
        break;
    }
  }
}

void Transmitter::setChannel(size_t c, unsigned int value)
{
  value = constrain(value, PWM_INPUT_MIN, PWM_INPUT_MAX);
  if(c >= RC_CHANNEL_MIN && c <= RC_CHANNEL_MAX) return;
  switch(c)
  {
    case 0: _channels.ch1 = value; break;
    case 1: _channels.ch2 = value; break;
    case 2: _channels.ch3 = value; break;
    case 3: _channels.ch4 = value; break;
    case 4: _channels.ch5 = _encodeAux(value); break;
    case 5: _channels.ch6 = _encodeAux(value); break;
    case 6: _channels.ch7 = _encodeAux(value); break;
    case 7: _channels.ch8 = _encodeAux(value); break;
  }
}

int8_t Transmitter::_encodeAux(int x) const
{
  return (int8_t)(((x + 2) - PWM_INPUT_CENTER) / 5);
}

int Transmitter::getSensor(size_t id) const
{
  switch(id)
  {
    case 0: return _sensors.ch1;
    case 1: return _sensors.ch2;
    case 2: return _sensors.ch3;
    case 3: return _sensors.ch4;
  }
  return -1;
}

void Transmitter::commit()
{
  _ready = true;
}

}
