#pragma once
#include <cstdint>
#include <SDL2/SDL.h>

struct AudioData
{
    int frequency;
};

class Audio
{
public:
    Audio(int soundFrequency);

    const AudioData& getAudioData() const;

    void setFrequency(int frequency);
    int getFrequency() const;

    void movePhase();
    float getPhase() const;

    void enable();
    void disable();

private:
    AudioData audioData;
    SDL_AudioDeviceID audioDevice;

    double phase = 0.0;
    double phaseStep;

    const int SAMPLE_FREQUENCY = 44100;
    const int BUFFER_SIZE = 512;

    static void callback(void* userdata, std::uint8_t* stream, int len);
};