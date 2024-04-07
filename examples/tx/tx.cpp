#include <Arduino.h>
#include "EspNowRcLink/Transmitter.h"

EspNowRcLink::Transmitter tx;

void setup()
{
  // initialize transmitter
  // - true to init hidden WiFi AP
  // - false if you manage WiFi outside (default)
  tx.begin(true);
}

void loop()
{
  uint32_t now = millis();
  static uint32_t sendNext = now + 20;

  // send rc channels every 20ms
  if(now > sendNext)
  {
    // use rc range 1000-2000
    for(size_t c = EspNowRcLink::RC_CHANNEL_MIN; c <= EspNowRcLink::RC_CHANNEL_MAX; c++)
    {
      tx.setChannel(c, 1000);
    }

    // mark as ready to send
    tx.commit();

    // schedule next transmission
    sendNext = now + 20;
  }

  // proceed sending and handle incoming data
  tx.update();
}
