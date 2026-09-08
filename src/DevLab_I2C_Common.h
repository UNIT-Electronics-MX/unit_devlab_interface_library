/*
  DevLab_I2C_Common.h

  Shared plumbing for every I2C bus variant (7-bit, 10-bit, ...):
  owning the TwoWire pointer, the clock, the leading-dummy-byte count,
  and begin()/setClock(). Plain inheritance, no virtuals - zero
  runtime cost, just removes the copy-paste between address-width
  variants. Address framing (the only thing that differs between
  7-bit and 10-bit) stays in the derived classes.

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#ifndef DEVLAB_I2C_COMMON_H
#define DEVLAB_I2C_COMMON_H

#include <Arduino.h>
#include <Wire.h>

class DevLab_I2C_Common {
protected:
    DevLab_I2C_Common(TwoWire &wire, uint32_t clock, uint8_t readDummyBytes);

    bool beginCommon();
    bool beginCommon(uint8_t sdaPin, uint8_t sclPin);
    void setClockCommon(uint32_t clock);

    TwoWire *_wire;
    uint32_t _clock;
    uint8_t _readDummyBytes;
};

#endif // DEVLAB_I2C_COMMON_H
