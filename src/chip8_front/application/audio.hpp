#pragma once
#include <SDL2/SDL.h>

class Audio
{
public:
    Audio(double soundFrequency);

    void setFrequency(int frequency);
    double getFrequency() const;

    void movePhase();
    double getPhase() const;

    void enable();
    void disable();

private:
    SDL_AudioDeviceID audioDevice;

    double soundFrequency;

    double phase = 0.0;
    double phaseStep;

    int SAMPLE_FREQUENCY = 44100;
    Uint16 BUFFER_SIZE = 512;

    static void callback(void* userdata, Uint8* stream, int len);
};