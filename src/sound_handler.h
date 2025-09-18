#ifndef SOUND_HANDLER_H
#define SOUND_HANDLER_H

#include <portaudio.h>
#include <vector>
#include <memory>
#include <mutex>
#include <functional>

class SoundHandler {
public:
    // Type alias for the audio callback function
    using AudioCallback = std::function<void(float* outputBuffer, unsigned long framesPerBuffer)>;

    SoundHandler();
    ~SoundHandler();

    // Prevent copying
    SoundHandler(const SoundHandler&) = delete;
    SoundHandler& operator=(const SoundHandler&) = delete;

    // Initialize the audio system
    bool initialize(int sampleRate = 44100, int framesPerBuffer = 256);

    // Start/stop the audio stream
    bool startStream();
    bool stopStream();

    // Set the audio callback function
    void setAudioCallback(AudioCallback callback);

    // Get current sample rate
    int getSampleRate() const { return sampleRate_; }

    // Get current frames per buffer
    int getFramesPerBuffer() const { return framesPerBuffer_; }

private:
    static int paCallback(const void* inputBuffer, void* outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void* userData);

    PaStream* stream_;
    int sampleRate_;
    int framesPerBuffer_;
    AudioCallback audioCallback_;
    std::mutex callbackMutex_;
    bool isInitialized_;
};

#endif // SOUND_HANDLER_H
