#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>

class Oscillator {
public:
    enum class Waveform {
        SINE,
        SQUARE,
        SAW,
        TRIANGLE
    };

    Oscillator(float sampleRate = 44100.0f);

    // Set waveform type
    void setWaveform(Waveform waveform) { waveform_ = waveform; }

    // Set frequency in Hz
    void setFrequency(float frequency) { frequency_ = frequency; }

    // Set amplitude (0.0 to 1.0)
    void setAmplitude(float amplitude) { amplitude_ = amplitude; }

    // Generate the next sample
    float process();

    // Operator overloading for fun!
    Oscillator& operator+=(float frequency) {
        frequency_ += frequency;
        return *this;
    }

    Oscillator& operator-=(float frequency) {
        frequency_ -= frequency;
        return *this;
    }

    float getFrequency() const { return frequency_; }
    float getAmplitude() const { return amplitude_; }
    Waveform getWaveform() const { return waveform_; }

private:
    float sampleRate_;
    float phase_;
    float frequency_;
    float amplitude_;
    Waveform waveform_;

    // Waveform generation functions
    float generateSine();
    float generateSquare();
    float generateSaw();
    float generateTriangle();
};

// Global operator overloads for Oscillator
Oscillator operator+(Oscillator osc, float frequency);
Oscillator operator-(Oscillator osc, float frequency);

#endif // OSCILLATOR_H
