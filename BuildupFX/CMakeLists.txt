cmake_minimum_required(VERSION 3.15)

project(BuildupFX VERSION 1.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as a subdirectory
# You need to have JUCE installed and set JUCE_DIR to point to it
# Example: cmake -DJUCE_DIR=/path/to/JUCE ..
if(NOT DEFINED JUCE_DIR)
    message(FATAL_ERROR "JUCE_DIR must be set to your JUCE installation directory")
endif()

add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/JUCE)

# Plugin formats to build
set(PLUGIN_FORMATS VST3 AU Standalone)

# Create the plugin target
juce_add_plugin(BuildupFX
    COMPANY_NAME "YourCompany"
    PLUGIN_MANUFACTURER_CODE YrCo
    PLUGIN_CODE BuFx
    FORMATS ${PLUGIN_FORMATS}
    PRODUCT_NAME "BuildupFX"

    # Plugin characteristics
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE

    # Plugin identifiers
    BUNDLE_ID com.yourcompany.BuildupFX

    # Copy plugin to system folders after building (optional)
    COPY_PLUGIN_AFTER_BUILD FALSE

    # Plugin channel configurations
    PLUGIN_CHANNEL_CONFIGURATIONS {2, 2}
)

# Add source files
target_sources(BuildupFX
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp

        # Header files (for IDE)
        Source/PluginProcessor.h
        Source/PluginEditor.h
        Source/DSP/NoiseGenerator.h
        Source/DSP/FilterModule.h
        Source/DSP/ReverbModule.h
        Source/DSP/DelayModule.h
        Source/DSP/DrumEngine.h
        Source/DSP/IntensityMapper.h
        Source/UI/CustomLookAndFeel.h
)

# Compiler definitions
target_compile_definitions(BuildupFX
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

# Link JUCE modules
target_link_libraries(BuildupFX
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Compiler options
if(MSVC)
    target_compile_options(BuildupFX PRIVATE /W4)
else()
    target_compile_options(BuildupFX PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Set optimization for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(BuildupFX PRIVATE /O2)
    else()
        target_compile_options(BuildupFX PRIVATE -O3)
    endif()
endif()
