# C++ Sound Synthesizer

A simple single channel sound synthesizer implemented in C++ with support for multiple waveforms and real-time audio processing.

## Features

- Waveform generation (sine, square, sawtooth, triangle)
- Audio processing with PortAudio
- Simple frequency control 
- Operator overloading for oscillator manipulation

## Dependencies

- C++20 or later
- CMake 3.26
- PortAudio 

## Installation

1. Install dependencies using Homebrew:
   ```bash
   brew install portaudio
   ```

2. Build the project:
   ```bash
   mkdir -p build && cd build
   cmake ..
   make
   ```

## Usage

Run the synthesizer:
```bash
./SoundSynthesizer
```

### Controls
- `s`: Switch to sine wave
- `w`: Switch to square wave
- `a`: Switch to sawtooth wave
- `t`: Switch to triangle wave
- `+`: Increase frequency by 50Hz
- `-`: Decrease frequency by 50Hz
- `q`: Quit the application

