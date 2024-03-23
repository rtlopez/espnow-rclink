
#include "EspNowRcLink/Receiver.h"

using namespace EspNowRcLink;

Receiver rx;

void setup()
{
  Serial.begin(115200);
  rx.begin();
}

void loop()
{
  uint32_t now = millis();
  static uint32_t rcDataNext = now + 500;

  rx.update();

  if(rx.available())
  {
    if(rcDataNext < now)
    {
      Serial.print("RC: ");
      for(size_t i = RC_CHANNEL_MIN; i <= RC_CHANNEL_MAX; i++)
      {
        Serial.print(rx.getChannel(i));
        Serial.print(' ');
      }
      Serial.println();
      rcDataNext = now + 500;
    }
  }
}
