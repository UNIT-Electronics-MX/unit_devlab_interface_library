/*
  DevLab_I2C_Interface.h

  Concrete I2C bus for the DevLab ecosystem. Implements the
  DevLab_BusIO contract (writeBytes/readBytes) using standard I2C
  register access: write register pointer, repeated start, read N
  bytes. Device-specific quirks (e.g. leading dummy bytes some chips
  return) belong to the sensor driver, not here.

  7-bit addressing only. For 10-bit addressed devices, use
  DevLab_I2C10_Interface instead - address framing differs enough
  between the two that keeping them as separate classes avoids a
  runtime branch on every transaction.

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#ifndef DEVLAB_I2C_INTERFACE_H
#define DEVLAB_I2C_INTERFACE_H

#include <Arduino.h>
#include <Wire.h>
#include "DevLab_BusIO.h"
#include "DevLab_I2C_Common.h"

class DevLab_I2C_Interface : public DevLab_BusIO<DevLab_I2C_Interface>, protected DevLab_I2C_Common {
public:
    // readDummyBytes: some chips (e.g. Bosch BMI323) prepend N filler
    // bytes to every register read before the real payload. Set it once
    // here and every helper (readRegisterN, getWhoAmI, ...) discards
    // them automatically - sensor drivers never touch byte offsets.
    DevLab_I2C_Interface(TwoWire &wire, uint8_t address, uint32_t clock = 400000,
                          uint8_t readDummyBytes = 0);

    // Uses whatever SDA/SCL the platform already has wired to `wire`.
    bool begin();

    // Explicit SDA/SCL pins (ESP32/RP2040-style begin overload).
    bool begin(uint8_t sdaPin, uint8_t sclPin);

    bool writeBytes(uint8_t reg, const uint8_t *data, size_t len);
    bool readBytes(uint8_t reg, uint8_t *data, size_t len);

    void setClock(uint32_t clock);
    void setAddress(uint8_t address);
    uint8_t address() const { return _address; }

    bool getWhoAmI(uint8_t reg, uint8_t &whoAmI);

private:
    uint8_t _address;
};

#endif // DEVLAB_I2C_INTERFACE_H
