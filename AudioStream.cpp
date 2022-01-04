#include "AudioStream.h"

AudioStream::AudioStream(int freqIn, int freqOut, int lrck, int ch1, int ch2, int ch3) {
    this->bitClockInputPin = freqIn;
    this->bitClockOutputPin = freqOut;
    this->frameClockOutputPin = lrck;
    this->audioDataCh1OutputPin = ch1;
    this->audioDataCh2OutputPin = ch2;
    this->audioDataCh3OutputPin = ch3;

    printf("%p\n", GPIO_Reg::gpio);

    GPIO_PinMode(freqIn, GPIO_INPUT);
    GPIO_PinMode(freqOut, GPIO_OUTPUT);
    GPIO_PinMode(lrck, GPIO_OUTPUT);
    GPIO_PinMode(ch1, GPIO_OUTPUT);
    GPIO_PinMode(ch2, GPIO_OUTPUT);
    GPIO_PinMode(ch3, GPIO_OUTPUT);

    isrGetterThread = new std::thread(&AudioStream::ISRGetterThreadRoutine, this);
    writerThread = new std::thread(&AudioStream::WriterThreadRoutine, this);
}

void AudioStream::ISRGetterThreadRoutine() {
    while(true) {
        //while(!GPIO_Get(bitClockInputPin));
        usleep(1);
        isrGot++;
        //while(GPIO_Get(bitClockInputPin));
        usleep(1);
        isrGot++;
    }
}

void AudioStream::WriterThreadRoutine() {
    while(true) {
        if(isrGot > isrProcessed) {
            if(clockState) {
                GPIO_Clr(bitClockOutputPin);
                clockState = false;
            } else {
                if(currentBit < 0) {
                    currentBit = 63;
                    currentBitMask = 0x8000000000000000;

                    if(queueReadPos < queueWritePos) {
                        uint64_t readPos = queueReadPos & 1048575;
                        ch1Data = queue_ch1[readPos];
                        ch2Data = queue_ch2[readPos];
                        ch3Data = queue_ch3[readPos];
                        queueReadPos++;
                    }
                }

                if(currentBit < 32) GPIO_Clr(frameClockOutputPin);
                else GPIO_Set(frameClockOutputPin);

                if(ch1Data & currentBitMask) GPIO_Set(audioDataCh1OutputPin);
                else GPIO_Clr(audioDataCh1OutputPin);
                if(ch2Data & currentBitMask) GPIO_Set(audioDataCh2OutputPin);
                else GPIO_Clr(audioDataCh2OutputPin);
                if(ch3Data & currentBitMask) GPIO_Set(audioDataCh3OutputPin);
                else GPIO_Clr(audioDataCh3OutputPin);

                currentBit--;
                currentBitMask >>= 1;

                GPIO_Set(bitClockOutputPin);
                clockState = true;
            }

            isrProcessed++;
        }
    }
}

void AudioStream::EnqueueSample(uint32_t l1, uint32_t r1, uint32_t l2, uint32_t r2, uint32_t l3, uint32_t r3) {
    uint64_t writePos = queueWritePos & 1048575;
    queue_ch1[writePos] = (uint64_t)l1 << 32ull | (uint64_t)r1;
    queue_ch2[writePos] = (uint64_t)l2 << 32ull | (uint64_t)r2;
    queue_ch3[writePos] = (uint64_t)l3 << 32ull | (uint64_t)r3;
    queueWritePos++;
}