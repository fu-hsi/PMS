# PMS Library
Arduino library for Plantower PMS sensors.
Supports PMS x003 sensors (1003, 3003, 5003, 6003, 7003).
## Installation
Just use Arduino Library Manager and search "PMS Library" in Sensors category.
## Main assumptions
- easy as possible,
- minimal memory consumption,
- non-blocking functions,
- supporting a wide range of PMS sensors from Plantower,
- supporting additional modes e.g.: sleeping, passive, active (depends on the sensor model).

As a data source you can use **any object** that implements the **Stream class**, such as Wire, Serial, EthernetClient, e.t.c.
## Basic example
Read in active mode.
> Default mode is active after power up. In this mode sensor would send serial data to the host automatically. The active mode is divided into two sub-modes: stable mode and fast mode. If the concentration change is small the sensor would run at stable mode with the real interval of 2.3s. And if the change is big the sensor would be changed to fast mode automatically with the interval of 200~800ms, the higher of the concentration, the shorter of the interval.
```cpp
#include "PMS.h"

PMS pms(Serial);
PMS::DATA data;

void setup()
{
  Serial.begin(9600);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial1.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
}

void loop()
{
  if (pms.read(data))
  {
    Serial1.print("PM 1.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_1_0);

    Serial1.print("PM 2.5 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_2_5);

    Serial1.print("PM 10.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_10_0);

    Serial1.println();
  }

  // Do other stuff...
}
```
## Output
```
PM 1.0 (ug/m3): 13
PM 2.5 (ug/m3): 18
PM 10.0 (ug/m3): 23

PM 1.0 (ug/m3): 12
PM 2.5 (ug/m3): 19
PM 10.0 (ug/m3): 24

...
```
## Advanced example
Read in passive mode but not the best way (see additional remarks). 
```cpp
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

  Serial1.println("Reading data...");
  if (pms.readUntil(data))
  {
    Serial1.print("PM 1.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_1_0);

    Serial1.print("PM 2.5 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_2_5);

    Serial1.print("PM 10.0 (ug/m3): ");
    Serial1.println(data.PM_AE_UG_10_0);
  }
  else
  {
    Serial1.println("No data.");
  }

  Serial1.println("Going to sleep for 60 seconds.");
  pms.sleep();
  delay(60000);
}
```
## Output
```
Waking up, wait 30 seconds for stable readings...
Send read request...
Reading data...
PM 1.0 (ug/m3): 13
PM 2.5 (ug/m3): 18
PM 10.0 (ug/m3): 23
Going to sleep for 60 seconds.
Waking up, wait 30 seconds for stable readings...
Send read request...
Reading data...
PM 1.0 (ug/m3): 12
PM 2.5 (ug/m3): 19
PM 10.0 (ug/m3): 24
Going to sleep for 60 seconds.

...
```
## Additional remarks
Tested with PMS 7003 and ESP-12E Development Board.
All Plantower PMS sensors uses the same protocol (let me know if you have any problems).

Please consider, that delay() function in examples is a blocking function.  
Try to avoid such a solution if your project requires it (see Expert.ino example in examples directory).

For more accurate measurements, you can read several samples (in passive or active mode) and calculate the average.
> Stable data should be got at least 30 seconds after the sensor wakeup from the sleep mode because of the fan's performance.