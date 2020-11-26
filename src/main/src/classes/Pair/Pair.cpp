/*
  Pair.cpp - Pair-Library for Librarys.
*/

#include "Arduino.h"
#include "Pair.h"

Pair::Pair(long val1, long val2) {
  this -> val1 = val1;
  this -> val2 = val2;
}

Pair::Pair(int val1, int val2) {
  this -> val1 = val1;
  this -> val2 = val2;
}

Pair::Pair(short val1, short val2) {
  this -> val1;
  this -> val2;
}

int Pair::getFirst() {
  return((int) val1);
}

int Pair::getLast() {
  return(val2);
}
