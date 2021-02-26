/*
  uint10_t.h - uint10_t-Library for Airduino.
*/

#ifndef uint10_t_h
#define uint10_t_h

#include "Arduino.h"

#define MINVALUE 300
#define SIZE 8

class uint10_t {
  public:
    uint10_t();
    uint10_t(int64_t var);
    uint10_t(uint64_t var);
    operator int();
    /*uint10_t operator=(int8_t var);
    uint10_t operator=(uint8_t var);
    uint10_t operator=(uint10_t var);
    uint10_t operator=(int16_t var);
    uint10_t operator=(uint16_t var);
    uint10_t operator=(int32_t var);
    uint10_t operator=(uint32_t var);*/
    uint10_t operator=(int64_t var);
    uint10_t operator=(uint64_t var);
    int operator=(uint10_t var);

  private:
    uint10_t setValue(int64_t var);
    uint10_t setValue(uint64_t var);
    int getValue();

    uint8_t value[SIZE];
};

#endif
