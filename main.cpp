#include <cstdio>
#include <cmath>
#include <unistd.h>

#include "GPIO.h"
#include "AudioStream.h"
#include "Network.h"
#include "DAC_Control.h"

volatile unsigned* GPIO_Reg::gpio;

struct VolumeControl {
    float fl, fr, c, sw, sl, sr;
} vol_ctrl;

int main() {
    if(GPIO_Init()) return 1;

    DAC_Control* ctrl = new DAC_Control(5, 6, 13, 19);
    ctrl->HardReset();
    AudioStream* astream = new AudioStream(26, 17, 18, 27, 22, 23);
    usleep(250000);
    ctrl->WriteReg(0x02, 0);
    ctrl->WriteReg(0x03, 0);
    usleep(250000);
    ctrl->WriteReg(0x09, 0);
    usleep(250000);
    ctrl->WriteReg(0x09, 1);
    Network* net = new Network();
    net->CreateSoundServer(8085);
    net->AcceptSoundRemote();
    vol_ctrl.fl = 1.0f;
    vol_ctrl.fr = 1.0f;
    vol_ctrl.c = 1.0f;
    vol_ctrl.sw = 1.0f;
    vol_ctrl.sl = 1.0f;
    vol_ctrl.sr = 1.0f;
    int16_t buf[6];
    while(true) {
        if(recv(net->soundSockfdRemote, (char*)buf, 12, 0) == 12) {
            astream->EnqueueSample(
                (buf[0] << 4),
                (buf[1] << 4),
                (buf[2] << 4),
                (buf[3] << 4),
                (buf[4] << 4),
                (buf[5] << 4)
            );
        }
    }
}