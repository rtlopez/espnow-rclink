#pragma once

#include <cstdint>

namespace EspNowRcLink {

enum MessageType: uint8_t {
  RC_DATA  = 0x01, // RC Channels (0x00-0x0f tx->rx)
  FC_ALIVE = 0x10, // FC Alive (0x10-0x1f rx->tx)
  FC_DATA  = 0x11, // FC Telemetry
  PAIR_REQ = 0xfe, // Pair Request (beacon rx->tx)
  PAIR_RES = 0xff, // Pair Response (pair tx->rx)
};

static const size_t WIFI_CHANNEL_MIN = 1;
static const size_t WIFI_CHANNEL_MAX = 13;
static const size_t WIFI_CHANNEL_DEFAULT = 7;

static const size_t RC_CHANNEL_MIN = 0;
static const size_t RC_CHANNEL_MAX = 7;

static const size_t PWM_INPUT_MIN = 880;
static const size_t PWM_INPUT_CENTER = 1500;
static const size_t PWM_INPUT_MAX = 2120;

static const size_t LINK_DISCOVERY_INTERVAL_MS = 200;
static const size_t LINK_BEACON_INTERVAL_MS = 50;
static const size_t LINK_ALIVE_INTERVAL_MS = 1000;

/*
  Protocol frame definition

  <type><payload><csum>

  - type: uint8_t - message type (mandatory)
  - payload: variable lenght [0-248] - message payload (optional)
  - csum: uint8_t - message checksum (mandatory)

  about checksum:
  - xor each byte except csum with 0x55 initial value (type and payload)
*/

struct MessageRc
{
  MessageType type = RC_DATA;
  int16_t ch1 = 1500;
  int16_t ch2 = 1500;
  int16_t ch3 = 1000;
  int16_t ch4 = 1500;
  int8_t ch5 = 0;
  int8_t ch6 = 0;
  int8_t ch7 = 0;
  int8_t ch8 = 0;
  uint8_t csum;
} __attribute__((packed));

struct MessageAlive
{
  MessageType type = FC_ALIVE;
  uint8_t csum;
} __attribute__((packed));

struct MessageFc
{
  MessageType type = FC_DATA;
  int16_t ch1 = 0;
  int16_t ch2 = 0;
  int8_t ch3 = 0;
  int8_t ch4 = 0;
  uint8_t csum;
} __attribute__((packed));

struct MessagePairRequest
{
  MessageType type = PAIR_REQ;
  uint8_t channel = WIFI_CHANNEL_DEFAULT;
  uint8_t csum;
} __attribute__((packed));

struct MessagePairResponse
{
  MessageType type = PAIR_RES;
  uint8_t csum;
} __attribute__((packed));

static const size_t PAYLOAD_SIZE_MIN = 2;
static const size_t PAYLOAD_SIZE_MAX = sizeof(MessageRc); // the biggest one

}
