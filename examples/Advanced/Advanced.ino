#include "PMS.h"

PMS pms(Serial);
PMS::DATA data;

void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  pms.passiveMode();    // Switch to passive mode
}

void loop()
{
  Serial1.println("Waking up, wait 30 seconds for stable readings...");
  pms.wakeUp();
  delay(30000);

  Serial1.println("Send read request...");
  pms.requestRead();

  Serial1.println("Wait max. 1 second for read...");
  if (pms.readUntil(data))
  {
    Serial1.print("PM 1.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_1_0);

    Serial1.print("PM 2.5 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_2_5);

    Serial1.print("PM 10.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_10_0);

    if (data.hasParticles)
    {
        Serial1.print("Particles > 0.3um: ");
        Serial1.println(data.PM_TOTALPARTICLES_0_3);
        Serial1.print("Particles > 0.5um: ");
        Serial1.println(data.PM_TOTALPARTICLES_0_5);
        Serial1.print("Particles > 1.0um: ");
        Serial1.println(data.PM_TOTALPARTICLES_1_0);
        Serial1.print("Particles > 2.5um: ");
        Serial1.println(data.PM_TOTALPARTICLES_2_5);
        Serial1.print("Particles > 5.0um: ");
        Serial1.println(data.PM_TOTALPARTICLES_5_0);
        Serial1.print("Particles > 10.0um: ");
        Serial1.println(data.PM_TOTALPARTICLES_10_0);
    }
  }
  else
  {
    Serial1.println("No data.");
  }

  Serial1.println("Going to sleep for 60 seconds.");
  pms.sleep();
  delay(60000);
}
