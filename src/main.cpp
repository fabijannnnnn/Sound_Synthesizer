#include <iostream>
#include <atomic>
#include <csignal>
#include <memory>
#include "sound_handler.h"
#include "oscillator.h"

std::atomic<bool> running{true};

// Signal handler for clean shutdown
void signalHandler(int signal) {
    if (signal == SIGINT) {
        running = false;
        std::cout << "\nReceived shutdown signal. Cleaning up..." << std::endl;
    }
}

int main() {
    // Set up signal handling
    std::signal(SIGINT, signalHandler);

    try {
        // Create and initialize the sound handler
        SoundHandler audio;

        // Create an oscillator
        Oscillator osc(audio.getSampleRate());
        osc.setWaveform(Oscillator::Waveform::SINE);
        osc.setFrequency(440.0f);  // A4 note
        osc.setAmplitude(0.3f);

        // Set up the audio callback
        audio.setAudioCallback([&osc](float* outputBuffer, unsigned long framesPerBuffer) {
            for (unsigned long i = 0; i < framesPerBuffer; i++) {
                outputBuffer[i] = osc.process();
            }
        });

        // Start the audio stream
        if (!audio.startStream()) {
            std::cerr << "Failed to start audio stream" << std::endl;
            return 1;
        }

        std::cout << "Sound synthesizer running. Press Ctrl+C to quit." << std::endl;
        std::cout << "Current note: A4 (440 Hz)" << std::endl;

        // Simple command loop
        while (running) {
            std::cout << "\nOptions:"
                      << "\n  q - Quit"
                      << "\n  s - Change to sine wave"
                      << "\n  w - Change to square wave"
                      << "\n  a - Change to sawtooth wave"
                      << "\n  t - Change to triangle wave"
                      << "\n  + - Increase frequency by 50 Hz"
                      << "\n  - - Decrease frequency by 50 Hz"
                      << "\nYour choice: ";

            char choice;
            std::cin >> choice;

            switch (choice) {
                case 'q':
                    running = false;
                    break;
                case 's':
                    osc.setWaveform(Oscillator::Waveform::SINE);
                    std::cout << "Waveform: Sine" << std::endl;
                    break;
                case 'w':
                    osc.setWaveform(Oscillator::Waveform::SQUARE);
                    std::cout << "Waveform: Square" << std::endl;
                    break;
                case 'a':
                    osc.setWaveform(Oscillator::Waveform::SAW);
                    std::cout << "Waveform: Sawtooth" << std::endl;
                    break;
                case 't':
                    osc.setWaveform(Oscillator::Waveform::TRIANGLE);
                    std::cout << "Waveform: Triangle" << std::endl;
                    break;
                case '+':
                    osc += 50.0f;  // Using overloaded operator
                    std::cout << "Frequency: " << osc.getFrequency() << " Hz" << std::endl;
                    break;
                case '-':
                    osc -= 50.0f;  // Using overloaded operator
                    std::cout << "Frequency: " << osc.getFrequency() << " Hz" << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice. Try again." << std::endl;
            }
        }

        // Clean up
        audio.stopStream();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
