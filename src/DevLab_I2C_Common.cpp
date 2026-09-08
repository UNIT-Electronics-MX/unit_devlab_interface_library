/*
  DevLab_I2C_Common.cpp

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#include "DevLab_I2C_Common.h"

DevLab_I2C_Common::DevLab_I2C_Common(TwoWire &wire, uint32_t clock, uint8_t readDummyBytes)
    : _wire(&wire), _clock(clock), _readDummyBytes(readDummyBytes) {
}

bool DevLab_I2C_Common::beginCommon() {
    _wire->begin();
    _wire->setClock(_clock);
    return true;
}

bool DevLab_I2C_Common::beginCommon(uint8_t sdaPin, uint8_t sclPin) {
#if defined(ARDUINO_ARCH_RP2040)
    // arduino-pico: pins are assigned before begin(), which takes no args.
    _wire->setSDA(sdaPin);
    _wire->setSCL(sclPin);
    _wire->setClock(_clock);
    _wire->begin();
#elif defined(ARDUINO_ARCH_ESP32)
    _wire->begin(sdaPin, sclPin, _clock);
#else
    (void)sdaPin;
    (void)sclPin;
    _wire->begin();
#endif
    _wire->setClock(_clock);
    return true;
}

void DevLab_I2C_Common::setClockCommon(uint32_t clock) {
    _clock = clock;
    _wire->setClock(clock);
}
