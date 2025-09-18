cmake_minimum_required(VERSION 3.26)
project(Synthesizer)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(Synthesizer
        src/main.cpp
        src/oscillator.h
        src/sound_handler.h
        src/oscillator.cpp
        src/sound_handler.cpp)

target_link_libraries(Synthesizer PRIVATE
        portaudio
        "-framework CoreAudio"
        "-framework AudioToolbox"
        "-framework AudioUnit"
        "-framework CoreServices"
        "-framework CoreFoundation"
)
