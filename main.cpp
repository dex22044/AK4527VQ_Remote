#include <cstdio>
#include <cmath>
#include <unistd.h>

#include "GPIO.h"
#include "AudioStream.h"

volatile unsigned* GPIO_Reg::gpio;

int main() {
    if(GPIO_Init()) return 1;

    AudioStream* astream = new AudioStream(4, 17, 18, 27, 22, 23);
    int frame = 0;
    while(true) {
        uint32_t sample = sin(frame / 500.0f) * 100000 + 100000;
        astream->EnqueueSample(sample, sample, sample, sample, sample, sample);
        frame++;
        usleep(100);
    }
}