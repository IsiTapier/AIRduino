/*
  Pair.h - Pair-Library for Airduino.
*/

#ifndef Pair_h
#define Pair_h

#include "Arduino.h"

class Pair {

  public:
    Pair(long val1, long val2);
    Pair(int val1, int val2);
    Pair(short val1, short val2);

    int getFirst();
    int getLast();

  private:
    long val1;
    long val2;

};

#endif
