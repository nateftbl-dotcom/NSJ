cmake_minimum_required(VERSION 3.15)

project(NullSpaceJenerator VERSION 0.1.0)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch JUCE if not present
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

# Plugin configuration
juce_add_plugin(NullSpaceJenerator
    COMPANY_NAME "NSJ"
    PLUGIN_MANUFACTURER_CODE Nsjx
    PLUGIN_CODE Nsjp
    
    FORMATS VST3 AU Standalone
    
    PRODUCT_NAME "Null Space Jenerator"
    
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    
    COPY_PLUGIN_AFTER_BUILD TRUE
    
    VST3_CATEGORIES Instrument Synth
    AU_MAIN_TYPE kAudioUnitType_MusicDevice
)

# Source files
target_sources(NullSpaceJenerator
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp
        src/SimpleSequencer.cpp
        src/SimpleSynth.cpp
)

# Include directories
target_include_directories(NullSpaceJenerator
    PRIVATE
        src
)

# Link JUCE modules
target_link_libraries(NullSpaceJenerator
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# Compile definitions
target_compile_definitions(NullSpaceJenerator
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
)

# Platform-specific settings
if(APPLE)
    target_compile_definitions(NullSpaceJenerator
        PUBLIC
            JUCE_PLUGINHOST_AU=1
    )
endif()

# Set output directory
set_target_properties(NullSpaceJenerator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
