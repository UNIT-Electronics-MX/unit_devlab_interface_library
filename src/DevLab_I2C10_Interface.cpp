/*
  DevLab_I2C10_Interface.cpp

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#include "DevLab_I2C10_Interface.h"

DevLab_I2C10_Interface::DevLab_I2C10_Interface(TwoWire &wire, uint16_t address10, uint32_t clock,
                                                uint8_t readDummyBytes)
    : DevLab_I2C_Common(wire, clock, readDummyBytes), _address10(address10 & 0x03FF) {
}

bool DevLab_I2C10_Interface::begin() {
    return beginCommon();
}

bool DevLab_I2C10_Interface::begin(uint8_t sdaPin, uint8_t sclPin) {
    return beginCommon(sdaPin, sclPin);
}

bool DevLab_I2C10_Interface::getWhoAmI(uint8_t reg, uint8_t &whoAmI) {
    return readRegister8(reg, whoAmI);
}

uint8_t DevLab_I2C10_Interface::addressHeader() const {
    return 0x78 | (uint8_t)((_address10 >> 8) & 0x03);
}

uint8_t DevLab_I2C10_Interface::addressLow() const {
    return (uint8_t)(_address10 & 0xFF);
}

bool DevLab_I2C10_Interface::writeBytes(uint8_t reg, const uint8_t *data, size_t len) {
    _wire->beginTransmission(addressHeader());
    _wire->write(addressLow());
    _wire->write(reg);
    if (len > 0 && data != nullptr) {
        _wire->write(data, len);
    }
    return (_wire->endTransmission() == 0);
}

bool DevLab_I2C10_Interface::readBytes(uint8_t reg, uint8_t *data, size_t len) {
    _wire->beginTransmission(addressHeader());
    _wire->write(addressLow());
    _wire->write(reg);
    if (_wire->endTransmission(false) != 0) {
        return false;
    }

    // Repeated start: resend only the header byte, R/W=1 this time.
    // requestFrom() appends the read bit itself.
    size_t totalLen = len + _readDummyBytes;
    size_t received = _wire->requestFrom(addressHeader(), (uint8_t)totalLen);
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

void DevLab_I2C10_Interface::setClock(uint32_t clock) {
    setClockCommon(clock);
}

void DevLab_I2C10_Interface::setAddress(uint16_t address10) {
    _address10 = address10 & 0x03FF;
}
