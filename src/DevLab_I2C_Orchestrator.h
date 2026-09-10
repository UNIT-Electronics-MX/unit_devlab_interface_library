/*
  DevLab_I2C_Orchestrator.h

    I2C bus orchestrator for the DevLab ecosystem. Unlike DevLab_I2C_Interface
    (bound to one fixed-address sensor), this class targets an arbitrary,
    changing address per call - the shape needed by bus-level protocols that
    scan, identify, and re-address devices at runtime (e.g. DevLab DDP).

    Transaction model differs from the register-style interface too: instead
    of write-register + repeated-start + read, it follows a command/response
    cycle - write a single command byte (STOP), allow the target device its
    processing delay, then issue a fresh read request. Devices on this bus
    decide when they're ready to answer; the orchestrator doesn't assume an
    immediate reply.

    Built on DevLab_I2C_Common for the shared Wire/clock/begin plumbing, same
    as DevLab_I2C_Interface and DevLab_I2C10_Interface - but exposes no
    register-read/write contract and holds no per-instance address, since a
    single orchestrator instance talks to every address on the bus.

  Organization:
  UNIT Electronics - DevLab Ecosystem

  Author:
  Jonathan Mejorado Lopez

  License:
  MIT License
*/

#ifndef DEVLAB_I2C_ORCHESTRATOR_H
#define DEVLAB_I2C_ORCHESTRATOR_H

#include <Arduino.h>
#include <Wire.h>
#include "DevLab_I2C_Common.h"

class DevLab_I2C_Orchestrator : protected DevLab_I2C_Common {
public:

    DevLab_I2C_Orchestrator(TwoWire &wire, uint32_t clock = 400000);

    bool begin();


    bool begin(uint8_t sdaPin, uint8_t sclPin);

    // Same as begin(sdaPin, sclPin), but first drives SCL/SDA to clock a
    // stuck slave off the bus (see DevLab_I2C_Common::devlabBeginI2CBusRecovered).
    // Use when a device may have been left mid-transaction (e.g. after a reset).
    bool beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs = 20000, bool restart = false);

    bool ping(uint8_t address);
    bool writeCommand(uint8_t address, uint8_t command);
    bool transact(uint8_t address, uint8_t command, uint8_t *data, size_t len, uint16_t delayMs = 0);
    void setClock(uint32_t clock);

};

#endif // DEVLAB_I2C_ORCHESTRATOR_H
