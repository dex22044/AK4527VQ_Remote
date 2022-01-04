#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

class GPIO_Reg {
public:
    static volatile unsigned* gpio;
};

#define INP_GPIO(g) *(GPIO_Reg::gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(GPIO_Reg::gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET *(GPIO_Reg::gpio+7)
#define GPIO_CLR *(GPIO_Reg::gpio+10)
#define GET_GPIO(g) (*(GPIO_Reg::gpio+13)&(1<<g))

inline int GPIO_Init() {
    int mem_fd = -1;
    if ((mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC) ) < 0) {
        printf("can't open /dev/gpiomem \n");
        return 1;
    }

    void* gpio_map = mmap(
        NULL,                 //Any adddress in our space will do
        4096,                 //Map length
        PROT_READ|PROT_WRITE, // Enable reading & writting to mapped memory
        MAP_SHARED,           //Shared with other processes
        mem_fd,               //File to map
        0x200000              //Offset to GPIO peripheral
    );

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map);
        return 1;
    }

    GPIO_Reg::gpio = (volatile unsigned *)gpio_map;

    return 0;
}

inline void GPIO_PinMode(int pin, int mode) {
    if(mode == GPIO_INPUT) {
        INP_GPIO(pin);
    }
    if(mode == GPIO_OUTPUT) {
        INP_GPIO(pin);
        OUT_GPIO(pin);
    }
}

inline void GPIO_Set(int pin) {
    GPIO_SET = 1 << pin;
}

inline void GPIO_Clr(int pin) {
    GPIO_CLR = 1 << pin;
}

inline bool GPIO_Get(int pin) {
    return GET_GPIO(pin);
}