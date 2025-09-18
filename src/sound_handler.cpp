#include "sound_handler.h"
#include <stdexcept>
#include <iostream>

SoundHandler::SoundHandler()
        : stream_(nullptr), sampleRate_(44100), framesPerBuffer_(256), isInitialized_(false) {

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        throw std::runtime_error("PortAudio initialization failed: " + std::string(Pa_GetErrorText(err)));
    }
}

SoundHandler::~SoundHandler() {
    if (stream_) {
        Pa_StopStream(stream_);
        Pa_CloseStream(stream_);
    }
    Pa_Terminate();
}

bool SoundHandler::initialize(int sampleRate, int framesPerBuffer) {
    if (isInitialized_) {
        return true;
    }

    sampleRate_ = sampleRate;
    framesPerBuffer_ = framesPerBuffer;

    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "Error: No default output device." << std::endl;
        return false;
    }

    outputParameters.channelCount = 1;  // Mono output
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = nullptr;

    PaError err = Pa_OpenStream(
            &stream_,
            nullptr,  // No input
            &outputParameters,
            sampleRate_,
            framesPerBuffer_,
            paClipOff,  // Don't clip samples
            &SoundHandler::paCallback,
            this  // Pass 'this' as user data
    );

    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    isInitialized_ = true;
    return true;
}

bool SoundHandler::startStream() {
    if (!isInitialized_) {
        if (!initialize(sampleRate_, framesPerBuffer_)) {
            return false;
        }
    }

    PaError err = Pa_StartStream(stream_);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool SoundHandler::stopStream() {
    if (!isInitialized_) {
        return true;
    }

    PaError err = Pa_StopStream(stream_);
    if (err != paNoError) {
        std::cerr << "Failed to stop stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

void SoundHandler::setAudioCallback(AudioCallback callback) {
    std::lock_guard<std::mutex> lock(callbackMutex_);
    audioCallback_ = std::move(callback);
}

int SoundHandler::paCallback(const void* inputBuffer, void* outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void* userData) {
    (void)inputBuffer;  // Unused
    (void)timeInfo;     // Unused
    (void)statusFlags;  // Unused

    auto* handler = static_cast<SoundHandler*>(userData);
    auto* out = static_cast<float*>(outputBuffer);

    std::lock_guard<std::mutex> lock(handler->callbackMutex_);
    if (handler->audioCallback_) {
        handler->audioCallback_(out, framesPerBuffer);
    } else {
        // Generate silence if no callback is set
        for (unsigned long i = 0; i < framesPerBuffer; i++) {
            out[i] = 0.0f;
        }
    }

    return paContinue;
}
