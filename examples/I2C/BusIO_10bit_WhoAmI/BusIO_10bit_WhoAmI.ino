/*
  DevLab_BusIO - 10-bit Addressing Validation Example

  Description:
  Exercises DevLab_I2C10_Interface::getWhoAmI() against an I2C device
  that uses 10-bit addressing (address range 0-1023) instead of the
  usual 7-bit scheme. Everything below the constructor call is
  identical to the 7-bit BusIO_WhoAmI example - writeBytes/readBytes
  handle the extra address-framing byte internally, so sensor-level
  code never has to know the difference.

  To test a real device, change:
    - I2C_ADDRESS10      (10-bit I2C address, e.g. 0x120)
    - WHO_AM_I_REG        (the register that holds the identity byte)
    - EXPECTED_WHO_AM_I    (the value the datasheet says to expect)

  Connections (adjust to your board/wiring):
  SDA -> GPIO 24
  SCL -> GPIO 25

  Organization:
  UNIT Electronics - DevLab Ecosystem

  License:
  MIT License
*/

#include <DevLab_I2C10_Interface.h>

#define SDA_PIN 24
#define SCL_PIN 25

#define I2C_ADDRESS10       0x120  // Example 10-bit address
#define WHO_AM_I_REG        0x00   // Replace with your device's identity register
#define EXPECTED_WHO_AM_I    0x00   // Replace with the value from your datasheet

DevLab_I2C10_Interface bus(Wire, I2C_ADDRESS10, 400000);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==================================================");
  Serial.println(" DevLab_BusIO - 10-bit Addressing Validation");
  Serial.println("==================================================");

  bus.begin(SDA_PIN, SCL_PIN);
  delay(50);

  uint8_t whoAmI;
  if (bus.getWhoAmI(WHO_AM_I_REG, whoAmI)) {
    Serial.print("WHO_AM_I: 0x");
    Serial.print(whoAmI, HEX);
    Serial.print("  (expected 0x");
    Serial.print(EXPECTED_WHO_AM_I, HEX);
    Serial.println(")");

    if (whoAmI == EXPECTED_WHO_AM_I) {
      Serial.println("[PASS] Device identity matches.");
    } else {
      Serial.println("[FAIL] Unexpected value - wrong device, address, or register.");
    }
  } else {
    Serial.println("[FAIL] getWhoAmI() returned false - no ACK from device.");
  }

  Serial.println("==================================================");
}

void loop() {
  // Nothing to do here - this sketch only validates getWhoAmI() on setup().
}
