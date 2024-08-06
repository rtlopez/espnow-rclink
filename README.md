# EspNow RcLink

RC Link library that works on ESP-NOW for tiny models in short range.

# Features

  * ESP32 and ESP8266 support
  * Auto pairing
  * Up to 100Hz packet rate (or maybe more)
  * 8 RC channels
  * Channels 1-4 are sent with full resolution and extended range 880-2120
  * Channels 5-8 are sent with reduced resolution and scaled for extended range 880-2120 (packed to 8-bit size, resolution is 5 units)
  * packet checksum checking

# How it works

  * when reciever is powered it sends pair request
  * when transmitter is powered, it scans channels for pair request
  * when transmitter receive pair request, adds peer to the list and sends pair response
  * when receiver recieive pair response, adds peer to the list and start communicating only with this transmitter
  * transmitter will not accept any other pair request, until power cycle
  * receiver will not accept any data packet form other transmitter, until power cycle

# Using in platformio

```ini
lib_deps =
  yoursunny/WifiEspNow @ ^0.0.20230713
  https://github.com/rtlopez/espnow-rclink/archive/refs/tags/v0.1.1.zip
```

# Examples

 * [Receiver](/examples/rx/rx.cpp)
 * [Transmitter](/examples/tx/tx.cpp)

# Todo

 * Tests, a lot of testing required
 * Telemetry
 * RSSI
 * More channels

# Licence

This project is distributed under MIT Licence.
