/*
  DevLab_I2C10_Interface.h

  Concrete I2C bus for the DevLab ecosystem, for devices addressed
  with the 10-bit I2C scheme (I2C-bus spec: reserved 7-bit header
  0b11110XX carries the two MSBs of the address, followed by a full
  byte carrying the 8 LSBs). Implements the same DevLab_BusIO
  contract (writeBytes/readBytes) as DevLab_I2C_Interface, but never
  branches on address width - kept as its own class instead of a
  runtime flag inside DevLab_I2C_Interface, so 7-bit transactions
  (the overwhelming majority of devices) never pay for a check they
  don't need.

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#ifndef DEVLAB_I2C10_INTERFACE_H
#define DEVLAB_I2C10_INTERFACE_H

#include <Arduino.h>
#include <Wire.h>
#include "DevLab_BusIO.h"
#include "DevLab_I2C_Common.h"

class DevLab_I2C10_Interface : public DevLab_BusIO<DevLab_I2C10_Interface>, protected DevLab_I2C_Common {
public:
    // address10: 10-bit device address (0-1023).
    // readDummyBytes: see DevLab_I2C_Interface.
    DevLab_I2C10_Interface(TwoWire &wire, uint16_t address10, uint32_t clock = 400000,
                            uint8_t readDummyBytes = 0);

    // Uses whatever SDA/SCL the platform already has wired to `wire`.
    bool begin();

    // Explicit SDA/SCL pins (ESP32/RP2040-style begin overload).
    bool begin(uint8_t sdaPin, uint8_t sclPin);

    bool writeBytes(uint8_t reg, const uint8_t *data, size_t len);
    bool readBytes(uint8_t reg, uint8_t *data, size_t len);

    void setClock(uint32_t clock);
    void setAddress(uint16_t address10);
    uint16_t address() const { return _address10; }

    bool getWhoAmI(uint8_t reg, uint8_t &whoAmI);

private:
    // First byte on the wire: 0b11110 + the 2 MSBs of the address.
    // beginTransmission()/requestFrom() append the R/W bit for us,
    // so this same 7-bit value doubles as the repeated-start header
    // used to switch a write into a read.
    uint8_t addressHeader() const;
    // Second byte on the wire: the 8 LSBs of the address.
    uint8_t addressLow() const;

    uint16_t _address10;
};

#endif // DEVLAB_I2C10_INTERFACE_H
