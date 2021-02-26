/*
 uint10_t.cpp - uint10_t-Library for Librarys.
*/

#include "uint10_t.h"

uint10_t::uint10_t() {}

uint10_t::uint10_t(int64_t var) {
    setValue(var);
}

uint10_t::uint10_t(uint64_t var) {
    setValue(var);
}

uint10_t::operator int() {
    getValue();
}

uint10_t uint10_t::operator=(int64_t var) {
    return setValue(var);
}

uint10_t uint10_t::operator=(uint64_t var) {
    return setValue(var);
}

int uint10_t::operator=(uint10_t var) {
    return getValue();
}

uint10_t uint10_t::setValue(int64_t var) {
    if(var < 0)
        var = 0;
    setValue((uint64_t) var);
}

uint10_t uint10_t::setValue(uint64_t var) {
    if(var > SIZE*256-1)
        var = SIZE*256-1;
    int i = 0;
    while(i <= SIZE && var != 0) {
        if(var >= 255) {
            value[i] = 255;
            var-=255;
        } else {
            value[i] = var;
            var = 0;
        }
    }
}

int uint10_t::getValue() {
    int out = 0;
    for(int i = 0; i <= SIZE; i++) {
        out+=value[i];
    }
    return out;
}