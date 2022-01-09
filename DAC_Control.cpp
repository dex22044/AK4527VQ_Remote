#include "DAC_Control.h"

DAC_Control::DAC_Control(int pdn, int csn, int cclk, int cdti) {
    this->pdnPin = pdn;
    this->csnPin = csn;
    this->cclkPin = cclk;
    this->cdtiPin = cdti;

    GPIO_PinMode(pdn, GPIO_OUTPUT);
    GPIO_PinMode(csn, GPIO_OUTPUT);
    GPIO_PinMode(cclk, GPIO_OUTPUT);
    GPIO_PinMode(cdti, GPIO_OUTPUT);

    GPIO_Set(csn);
    GPIO_Set(cclk);
    GPIO_Clr(cdti);
}

void DAC_Control::HardReset() {
    GPIO_Set(pdnPin);
    usleep(50000);
    GPIO_Clr(pdnPin);
    usleep(250000);
    GPIO_Set(pdnPin);
    usleep(250000);
}

void DAC_Control::WriteRaw(uint16_t data) {
    GPIO_Clr(cdtiPin);
    usleep(10);
    GPIO_Clr(csnPin);
    usleep(1);
    for(int i = 15; i >= 0; i--) {
        GPIO_Clr(cclkPin);
        usleep(40);
        bool d = data & (1 << i);
        if(d) GPIO_Set(cdtiPin);
        else GPIO_Clr(cdtiPin);
        usleep(10);
        GPIO_Set(cclkPin);
        usleep(10);
    }
    usleep(10);
    GPIO_Set(csnPin);
    GPIO_Set(cclkPin);
    GPIO_Clr(cdtiPin);
    usleep(100);
}

void DAC_Control::WriteReg(char regAddr, char data) {
    regAddr &= 0b11111;
    uint16_t d = 0b0010000000000000;
    d |= regAddr << 8;
    d |= data;
    WriteRaw(d);
}