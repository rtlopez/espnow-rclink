#include <Arduino.h>
#include "EspNowRcLink/Receiver.h"

EspNowRcLink::Receiver rx;

void setup()
{
  // initialize receiver
  // - true to init hidden WiFi AP
  // - false if you manage WiFi outside (default)
  rx.begin(true);
}

void loop()
{
  // receive data
  rx.update();

  // check if new data available
  if(rx.available())
  {
    for(size_t c = EspNowRcLink::RC_CHANNEL_MIN; c <= EspNowRcLink::RC_CHANNEL_MAX; c++)
    {
      uint16_t v = rx.getChannel(c);
      //proceedRcChannel(c, v);
    }
  }
}
