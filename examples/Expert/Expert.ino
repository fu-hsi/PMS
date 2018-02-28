#include "PMS.h"

// To use Deep Sleep connect RST to GPIO16 (D0) and uncomment below line.
// #define DEEP_SLEEP

// GPIO2 (D4 pin on ESP-12E Development Board).
#define DEBUG_OUT Serial1

// PMS_READ_INTERVAL (4:30 min) and PMS_READ_DELAY (30 sec) CAN'T BE EQUAL! Values are also used to detect sensor state.
static const uint32_t PMS_READ_INTERVAL = 270000;
static const uint32_t PMS_READ_DELAY = 30000;

// Default sensor state.
uint32_t timerInterval = PMS_READ_DELAY;

PMS pms(Serial);

void setup()
{
  // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial.begin(PMS::BAUD_RATE); 
  DEBUG_OUT.begin(9600);

  // Switch to passive mode.
  pms.passiveMode();

  // Default state after sensor power, but undefined after ESP restart e.g. by OTA flash, so we have to manually wake up the sensor for sure.
  // Some logs from bootloader is sent via Serial port to the sensor after power up. This can cause invalid first read or wake up so be patient and wait for next read cycle.
  pms.wakeUp();

#ifdef DEEP_SLEEP
  delay(PMS_READ_DELAY);
  readData();
  pms.sleep();
  ESP.deepSleep(PMS_READ_INTERVAL * 1000);
#endif // DEEP_SLEEP
}

void loop()
{
#ifndef DEEP_SLEEP
  static uint32_t timerLast = 0;

  uint32_t timerNow = millis();
  if (timerNow - timerLast >= timerInterval) {
    timerLast = timerNow;
    timerCallback();
    timerInterval = timerInterval == PMS_READ_DELAY ? PMS_READ_INTERVAL : PMS_READ_DELAY;
  }

  // Do other stuff...
#endif // DEEP_SLEEP
}

#ifndef DEEP_SLEEP
void timerCallback() {
  if (timerInterval == PMS_READ_DELAY)
  {
    readData();
    DEBUG_OUT.println("Going to sleep.");
    pms.sleep();
  }
  else
  {
    DEBUG_OUT.println("Waking up.");
    pms.wakeUp();
  }
}
#endif // DEEP_SLEEP

void readData()
{
  PMS::DATA data;

  // Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
  while (Serial.available()) { Serial.read(); }

  DEBUG_OUT.println("Send read request...");
  pms.requestRead();

  DEBUG_OUT.println("Reading data...");
  if (pms.readUntil(data))
  {
    DEBUG_OUT.println();

    DEBUG_OUT.print("PM 1.0 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_1_0);

    DEBUG_OUT.print("PM 2.5 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_2_5);

    DEBUG_OUT.print("PM 10.0 (ug/m3): ");
    DEBUG_OUT.println(data.PM_AE_UG_10_0);

    DEBUG_OUT.println();
  }
  else
  {
    DEBUG_OUT.println("No data.");
  }
}