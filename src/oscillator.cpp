#include "oscillator.h"
#include <stdexcept>

Oscillator::Oscillator(float sampleRate)
        : sampleRate_(sampleRate), phase_(0.0f), frequency_(440.0f),
          amplitude_(0.5f), waveform_(Waveform::SINE) {
    if (sampleRate <= 0) {
        throw std::invalid_argument("Sample rate must be greater than 0");
    }
}

float Oscillator::process() {
    float sample = 0.0f;

    switch (waveform_) {
        case Waveform::SINE:
            sample = generateSine();
            break;
        case Waveform::SQUARE:
            sample = generateSquare();
            break;
        case Waveform::SAW:
            sample = generateSaw();
            break;
        case Waveform::TRIANGLE:
            sample = generateTriangle();
            break;
    }

    // Update phase for next sample
    phase_ += frequency_ / sampleRate_;
    if (phase_ >= 1.0f) {
        phase_ -= 1.0f;
    }

    return sample * amplitude_;
}

float Oscillator::generateSine() {
    return std::sin(2.0f * M_PI * phase_);
}

float Oscillator::generateSquare() {
    return (phase_ < 0.5f) ? 1.0f : -1.0f;
}

float Oscillator::generateSaw() {
    return 2.0f * phase_ - 1.0f;
}

float Oscillator::generateTriangle() {
    float value = -1.0f + (2.0f * phase_);
    return 2.0f * (std::fabs(value) - 0.5f);
}

// Global operator overloads
Oscillator operator+(Oscillator osc, float frequency) {
    osc.setFrequency(osc.getFrequency() + frequency);
    return osc;
}

Oscillator operator-(Oscillator osc, float frequency) {
    osc.setFrequency(osc.getFrequency() - frequency);
    return osc;
}
