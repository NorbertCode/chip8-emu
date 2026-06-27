#include "audio.hpp"
#include <iostream>
#include <cmath>
#include <numbers>

const double TWO_PI = 2.0 * std::numbers::pi;

Audio::Audio(double soundFrequency)
    : soundFrequency(soundFrequency)
{
    SDL_AudioSpec spec = {
        .freq = SAMPLE_FREQUENCY,
        .format = AUDIO_F32,
        .channels = 1,
        .samples = BUFFER_SIZE,
        .callback = callback,
        .userdata = this
    };

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
    if (!audioDevice)
    {
        std::cerr << "Error opening audio device: " << SDL_GetError() << std::endl;
        return;
    }

    phaseStep = TWO_PI * soundFrequency / SAMPLE_FREQUENCY;
}

void Audio::setFrequency(int frequency)
{
    SDL_LockAudioDevice(audioDevice);

    soundFrequency = frequency;
    phaseStep = TWO_PI * frequency / SAMPLE_FREQUENCY;

    SDL_UnlockAudioDevice(audioDevice);
}

int Audio::getFrequency() const
{
    return soundFrequency;
}

void Audio::movePhase()
{
    phase += phaseStep;

    if (phase >= TWO_PI)
        phase -= TWO_PI;
}

float Audio::getPhase() const
{
    return phase;
}

void Audio::enable()
{
    SDL_PauseAudioDevice(audioDevice, 0);
}

void Audio::disable()
{
    SDL_PauseAudioDevice(audioDevice, 1);
}

void Audio::callback(void* userdata, std::uint8_t* stream, int len)
{
    Audio* audio = static_cast<Audio*>(userdata);
    float* fstream = reinterpret_cast<float*>(stream);

    for (size_t i = 0; i < len / sizeof(float); ++i)
    {
        fstream[i] = std::sin(TWO_PI + audio->getPhase());
        audio->movePhase();
    }
}