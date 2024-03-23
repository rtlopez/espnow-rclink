#include <Arduino.h>
#include "EspNowRcLink/Transmitter.h"

// uncomment to activate simultor on channel 3
//#define ESPNOW_RCLINK_DEMO_TX_SIM

using namespace EspNowRcLink;

Transmitter tx;

void setup()
{
  Serial.begin(115200);

  tx.begin();

  tx.setChannel(0, 1500);
  tx.setChannel(1, 1500);
  tx.setChannel(2, 1000);
  tx.setChannel(3, 1500);
  tx.setChannel(4, 1500);
  tx.setChannel(5, 1500);
  tx.setChannel(6, 1500);
  tx.setChannel(7, 1500);
}

void loop()
{
  static int sim_cnt = 0;
  static bool sim_up = true;
  int sim_speed = 4;

  uint32_t now = millis();
  static uint32_t rcDataNext = now + 50;

  // send rc channels
  if(rcDataNext < now)
  {
    tx.setChannel(2, 1000 + (sim_cnt * sim_speed));
    tx.commit();
    rcDataNext = now + 20;

#ifdef ESPNOW_RCLINK_DEMO_TX_SIM
    if(sim_up)
    {
      sim_cnt++;
      if(sim_cnt >= (1000 / sim_speed))
      {
        sim_up = false;
      }
    }
    else
    {
      sim_cnt--;
      if(sim_cnt == 0)
      {
        sim_up = true;
      }
    }
#else
    (void)sim_up;
#endif
  }
  tx.update();
}