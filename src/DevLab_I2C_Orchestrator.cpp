/*
  DevLab_I2C_Orchestrator.cpp

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#include "DevLab_I2C_Orchestrator.h"

DevLab_I2C_Orchestrator::DevLab_I2C_Orchestrator(TwoWire &wire, uint32_t clock)
    : DevLab_I2C_Common(wire, clock, 0) {
}

bool DevLab_I2C_Orchestrator::begin() {
    return beginCommon();
}

bool DevLab_I2C_Orchestrator::begin(uint8_t sdaPin, uint8_t sclPin) {
    return beginCommon(sdaPin, sclPin);
}

bool DevLab_I2C_Orchestrator::beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs, bool restart) {
    return devlabBeginI2CBusRecovered(sdaPin, sclPin, _clock, timeoutUs, restart);
}

void DevLab_I2C_Orchestrator::setClock(uint32_t clock) {
    setClockCommon(clock);
}

bool DevLab_I2C_Orchestrator::ping(uint8_t address) {
    _wire->beginTransmission(address);
    return (_wire->endTransmission() == 0);
}

bool DevLab_I2C_Orchestrator::writeCommand(uint8_t address, uint8_t command) {
    _wire->beginTransmission(address);
    _wire->write(command);
    return (_wire->endTransmission() == 0);
}

bool DevLab_I2C_Orchestrator::transact(uint8_t address, uint8_t command, uint8_t *data, size_t len, uint16_t delayMs) {
    if (!writeCommand(address, command)) {
        return false;
    }
    if (delayMs > 0) {
        delay(delayMs);
    }
    uint8_t received = 0;
    received = _wire->requestFrom(address, (uint8_t)len);

    if (received != len) {
      while (_wire->available()) _wire->read();
      return false;
    }

    for (size_t i = 0; i < len; i++) {
        data[i] = _wire->read();
    }
    return true;
}