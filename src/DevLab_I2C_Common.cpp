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

static constexpr uint8_t  MAX_RECOVERY_PULSES  = 18;
static constexpr uint32_t HALF_PERIOD_US        = 5;


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

bool DevLab_I2C_Common::devlabWaitForI2CLineHigh(uint8_t pin, uint32_t timeoutUs) {

    if(digitalRead(pin)) {
        return true; // Line is already high , no blocking 
    }
    const uint32_t started = micros();
    while(!digitalRead(pin)) {
        if ((uint32_t)(micros() - started) >= timeoutUs) {
            return false; // Timeout
        }
        delayMicroseconds(10); // Space out polling; still busy-waits, just less tightly
    }
    return true; // Line is high
}

bool DevLab_I2C_Common::devlabClearI2CBus(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs) {
    
    const uint32_t deadline = micros() + timeoutUs;
    auto remainingUs = [deadline]() -> uint32_t {
        int32_t left = (int32_t)(deadline - micros());
        return left > 0 ? (uint32_t)left : 0;
    };

    pinMode(sdaPin, INPUT_PULLUP);
    pinMode(sclPin, INPUT_PULLUP);
    delay(2);
    if (!devlabWaitForI2CLineHigh(sclPin, remainingUs())) return false;

    for (uint8_t pulse = 0; pulse < MAX_RECOVERY_PULSES && !digitalRead(sdaPin); ++pulse) {
        digitalWrite(sclPin, LOW);
        pinMode(sclPin, OUTPUT);
        delayMicroseconds(HALF_PERIOD_US);
        pinMode(sclPin, INPUT_PULLUP);
        if (!devlabWaitForI2CLineHigh(sclPin, remainingUs())) return false;
        delayMicroseconds(HALF_PERIOD_US);
    }

    digitalWrite(sclPin, LOW);
    pinMode(sclPin, OUTPUT);
    digitalWrite(sdaPin, LOW);
    pinMode(sdaPin, OUTPUT);
    delayMicroseconds(HALF_PERIOD_US);
    pinMode(sclPin, INPUT_PULLUP);
    if (!devlabWaitForI2CLineHigh(sclPin, remainingUs())) {
        pinMode(sdaPin, INPUT_PULLUP);
        return false;
    }
    delayMicroseconds(HALF_PERIOD_US);
    pinMode(sdaPin, INPUT_PULLUP);
    delayMicroseconds(HALF_PERIOD_US);
    return digitalRead(sdaPin) && digitalRead(sclPin);
}

bool DevLab_I2C_Common::devlabBeginI2CBusRecovered(uint8_t sdaPin, uint8_t sclPin,
    uint32_t frequency, uint32_t timeoutUs, bool restart) {
        if (restart) {
            _wire->end();
            delay(2);
        }
        if (!devlabClearI2CBus(sdaPin, sclPin, timeoutUs)) return false;

    _clock = frequency;
    return beginCommon(sdaPin, sclPin);
}
