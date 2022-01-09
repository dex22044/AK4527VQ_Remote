#pragma once

#include "GPIO.h"
#include <unistd.h>
#include <cstdint>

class DAC_Control {
private:
    int pdnPin;
    int csnPin;
    int cclkPin;
    int cdtiPin;

public:
    DAC_Control(int pdn, int csn, int cclk, int cdti);
    void HardReset();
    void WriteRaw(uint16_t data);
    void WriteReg(char regAddr, char data);
};