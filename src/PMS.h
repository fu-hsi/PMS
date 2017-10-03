#ifndef PMS_H
#define PMS_H

#include "Stream.h"

class PMS
{
public:
  struct DATA {
    // Factory environment 
    uint16_t PM_FE_UG_1_0;
    uint16_t PM_FE_UG_2_5;
    uint16_t PM_FE_UG_10_0;

    // Atmospheric environment
    uint16_t PM_AE_UG_1_0;
    uint16_t PM_AE_UG_2_5;
    uint16_t PM_AE_UG_10_0;
  };

  enum STATUS { STATUS_WAITING, STATUS_OK };
  enum MODE { MODE_ACTIVE, MODE_PASSIVE };

  PMS(Stream&);
  void sleep();
  void wakeUp();
  void activeMode();
  void passiveMode();

  void requestRead();
  bool read(DATA& data, uint16_t timeout = 0);

private:
  uint8_t _payload[12];
  Stream* _stream;
  DATA* _data;
  STATUS _status;
  MODE _mode = MODE::MODE_ACTIVE;
  
  uint8_t _index = 0;
  uint16_t _frameLen;
  uint16_t _checksum;
  uint16_t _calculatedChecksum;

  void loop();
};

#endif