#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

class Audio
{
public:
    Audio(double soundFrequency);

    void setFrequency(int frequency);
    int getFrequency() const;

    void movePhase();
    float getPhase() const;

    void enable();
    void disable();

private:
    SDL_AudioDeviceID audioDevice;

    double soundFrequency;

    double phase = 0.0;
    double phaseStep;

    const int SAMPLE_FREQUENCY = 44100;
    const Uint16 BUFFER_SIZE = 512;

    static void callback(void* userdata, Uint8* stream, int len);
};