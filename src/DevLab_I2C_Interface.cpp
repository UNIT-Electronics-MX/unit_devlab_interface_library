/*
  DevLab_I2C_Interface.cpp

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#include "DevLab_I2C_Interface.h"

DevLab_I2C_Interface::DevLab_I2C_Interface(TwoWire &wire, uint8_t address, uint32_t clock,
                                            uint8_t readDummyBytes)
    : DevLab_I2C_Common(wire, clock, readDummyBytes), _address(address) {
}

bool DevLab_I2C_Interface::begin() {
    return beginCommon();
}

bool DevLab_I2C_Interface::begin(uint8_t sdaPin, uint8_t sclPin) {
    return beginCommon(sdaPin, sclPin);
}

bool DevLab_I2C_Interface::getWhoAmI(uint8_t reg, uint8_t &whoAmI) {
    return readRegister8(reg, whoAmI);
}

bool DevLab_I2C_Interface::writeBytes(uint8_t reg, const uint8_t *data, size_t len) {
    _wire->beginTransmission(_address);
    _wire->write(reg);
    if (len > 0 && data != nullptr) {
        _wire->write(data, len);
    }
    return (_wire->endTransmission() == 0);
}

bool DevLab_I2C_Interface::readBytes(uint8_t reg, uint8_t *data, size_t len) {
    _wire->beginTransmission(_address);
    _wire->write(reg);
    if (_wire->endTransmission(false) != 0) {
        return false;
    }

    size_t totalLen = len + _readDummyBytes;
    size_t received = _wire->requestFrom(_address, (uint8_t)totalLen);
    if (received < totalLen) {
        return false;
    }

    for (uint8_t i = 0; i < _readDummyBytes; i++) {
        _wire->read(); // discard chip-specific filler bytes
    }
    for (size_t i = 0; i < len; i++) {
        data[i] = _wire->read();
    }
    return true;
}

void DevLab_I2C_Interface::setClock(uint32_t clock) {
    setClockCommon(clock);
}

void DevLab_I2C_Interface::setAddress(uint8_t address) {
    _address = address;
}
