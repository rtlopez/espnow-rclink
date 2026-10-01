#pragma once

#include <cstddef>
#include <cstdint>
#include "Protocol.h"

namespace EspNowRcLink {

struct Message
{
  uint8_t mac[MAC_LEN];
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

}
