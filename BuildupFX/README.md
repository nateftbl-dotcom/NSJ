# BuildupFX - VST3/AU Plugin

A professional buildup effect plugin for house music production, inspired by the Pioneer RMX-1000. Built with JUCE framework.

## Features

### Core Functionality
- **Two Main Intensity Controls**
  - **NOISE/FX Intensity**: Controls atmospheric buildup layers (white/pink noise, filter sweeps, reverb, delay)
  - **DRUM Intensity**: Controls rhythmic buildup layers (snare rolls, kicks, claps)

- **Flexible Buildup Lengths**: 4, 8, 16, or 32 bars
- **Manual Trigger System**: One-button buildup activation
- **Auto-Retrigger**: Automatically start new buildup after drop
- **Visual Feedback**: Real-time progress bar with color-coded intensity

### Noise/FX Section
Progressive effect layering based on intensity:
- **Low (0-30%)**: Subtle white noise + gentle HPF sweep + light reverb
- **Medium (30-70%)**: Pink noise blend + stronger filtering + delay buildup
- **High (70-100%)**: Full noise + aggressive HPF/LPF bracket + near-oscillation delay + pitch rise

**Presets**: Riser, Dark Tension, Bright Energy, Filtered

### Drum Section
Progressive pattern density based on intensity:
- **Low (0-30%)**: Sparse hits (every 2 bars)
- **Medium (30-70%)**: Regular hits (every bar) + claps
- **High (70-100%)**: Progressive rolls (quarter → eighth → sixteenth → thirty-second notes)

**Presets**: Snare Roll, Full Kit, Minimal, Kick Build

### Drop Effects
- **None**: Clean stop
- **Vinyl Brake**: Gradual slowdown effect
- **Filter Drop**: Instant HPF reset
- **Impact**: Transient punch at drop

## Requirements

### Build Requirements
- **JUCE Framework** 7.0 or later
- **CMake** 3.15+ (if using CMake build)
- **Compiler**:
  - macOS: Xcode 12+
  - Windows: Visual Studio 2022
  - Linux: GCC 9+ or Clang 10+

### Runtime Requirements
- VST3 or AU compatible DAW
- macOS 10.13+, Windows 10+, or Linux

## Building the Plugin

### Method 1: Using Projucer (Recommended)

1. **Install JUCE**
   ```bash
   # Download from https://juce.com/get-juce/download
   # Or clone from GitHub:
   git clone https://github.com/juce-framework/JUCE.git
   ```

2. **Set up JUCE global paths**
   - Open Projucer
   - Go to File → Global Paths
   - Set "Path to JUCE" to your JUCE installation directory

3. **Open the project**
   ```bash
   cd BuildupFX
   # Open BuildupFX.jucer with Projucer
   ```

4. **Generate platform-specific project**
   - In Projucer, select your target platform (Xcode/Visual Studio/Linux Makefile)
   - Click "Save and Open in IDE"

5. **Build in your IDE**
   - **macOS (Xcode)**: Select "BuildupFX - VST3" or "BuildupFX - AU" scheme, then Product → Build
   - **Windows (Visual Studio)**: Select "Release" configuration, then Build → Build Solution
   - **Linux**: Run `make` in the Builds/LinuxMakefile directory

### Method 2: Using CMake

1. **Install JUCE**
   ```bash
   git clone https://github.com/juce-framework/JUCE.git ~/JUCE
   cd ~/JUCE
   git checkout 7.0.0  # Or latest stable version
   ```

2. **Build the plugin**
   ```bash
   cd BuildupFX
   mkdir build
   cd build

   # Configure (replace /path/to/JUCE with your JUCE location)
   cmake -DJUCE_DIR=~/JUCE -DCMAKE_BUILD_TYPE=Release ..

   # Build
   cmake --build . --config Release
   ```

3. **Install the plugin**
   - **macOS**: Copy from `build/BuildupFX_artefacts/Release/VST3/` to `~/Library/Audio/Plug-Ins/VST3/`
   - **Windows**: Copy from `build\BuildupFX_artefacts\Release\VST3\` to `C:\Program Files\Common Files\VST3\`
   - **Linux**: Copy from `build/BuildupFX_artefacts/Release/VST3/` to `~/.vst3/`

## Installation

### VST3
- **macOS**: `~/Library/Audio/Plug-Ins/VST3/BuildupFX.vst3`
- **Windows**: `C:\Program Files\Common Files\VST3\BuildupFX.vst3`
- **Linux**: `~/.vst3/BuildupFX.vst3`

### AU (macOS only)
- `~/Library/Audio/Plug-Ins/Components/BuildupFX.component`

After copying the plugin, restart your DAW.

## Usage

### Basic Workflow

1. **Insert BuildupFX** on your master channel or group bus

2. **Select Buildup Length**
   - Choose 4, 8, 16, or 32 bars using the radio buttons
   - Default: 16 bars

3. **Set Intensity Levels**
   - Adjust **NOISE/FX Intensity** (0-100%) for atmospheric buildup
   - Adjust **DRUM Intensity** (0-100%) for rhythmic buildup
   - These set the *target* intensity - actual intensity ramps from 0 to your setting

4. **Trigger the Buildup**
   - Click the **TRIGGER** button at the desired starting point
   - The plugin syncs to your DAW's tempo automatically
   - Watch the progress bar for visual feedback

5. **Buildup Executes**
   - Effects gradually increase from 0% to your set intensity levels
   - Progress bar shows remaining bars
   - Color changes from green → yellow → red as intensity increases

6. **Drop Happens**
   - At the end of the buildup length, the drop effect triggers
   - Select drop type: None, Vinyl Brake, Filter Drop, or Impact

7. **Optional: Auto-Retrigger**
   - Enable "Auto-Retrigger" checkbox to automatically start a new buildup after each drop

### Tips for Best Results

**Intensity Settings:**
- For subtle buildups: Noise 30-50%, Drums 40-60%
- For aggressive buildups: Noise 80-100%, Drums 90-100%
- Experiment with different intensity balances (high noise + low drums, or vice versa)

**Preset Combinations:**
- **Classic House**: Noise "Riser" + Drums "Snare Roll"
- **Dark & Heavy**: Noise "Dark Tension" + Drums "Kick Build"
- **Light & Airy**: Noise "Bright Energy" + Drums "Minimal"
- **Filter Focus**: Noise "Filtered" + Drums "Minimal"

**Mix Controls:**
- **Noise Mix**: Controls volume of generated noise (vs. filtered input)
- **Sidechain Duck**: Reduces input audio during buildup (makes buildup more prominent)
- **Drum Mix**: Controls volume of generated drums
- **Humanize**: Adds velocity/timing variation to drum hits (0% = robotic, 100% = loose)

**Output:**
- Use **Output Level** to compensate for loudness changes
- Use **Dry/Wet** to blend effect with unprocessed signal (100% wet recommended for master channel use)

## Architecture

### DSP Modules

- **NoiseGenerator.h**: White/pink noise generation with pitch shifting
- **FilterModule.h**: Dual HPF/LPF with smooth parameter ramping
- **ReverbModule.h**: Reverb wrapper with dynamic room size
- **DelayModule.h**: Tempo-synced delay with feedback control
- **DrumEngine.h**: Sample playback and pattern generation
- **IntensityMapper.h**: Maps intensity (0-100) to DSP parameters using curves

### Processing Flow

```
Input Audio
    │
    ├─→ Filter Sweep (HPF/LPF based on noise intensity)
    ├─→ Reverb (expanding based on noise intensity)
    ├─→ Delay (feedback increasing based on noise intensity)
    ├─→ Sidechain Duck (reduces input during buildup)
    │
    ├─→ Noise Generator (generates white/pink noise)
    │   └─→ Mixed into signal based on Noise Mix parameter
    │
    ├─→ Drum Engine (generates drum hits based on drum intensity)
    │   └─→ Mixed into signal based on Drum Mix parameter
    │
    └─→ Output Level → Dry/Wet Mix → Output
```

## Project Structure

```
BuildupFX/
├── Source/
│   ├── DSP/
│   │   ├── NoiseGenerator.h       # Noise generation
│   │   ├── FilterModule.h         # HPF/LPF filtering
│   │   ├── ReverbModule.h         # Reverb effect
│   │   ├── DelayModule.h          # Delay effect
│   │   ├── DrumEngine.h           # Drum sample playback
│   │   └── IntensityMapper.h      # Parameter mapping curves
│   ├── UI/
│   │   └── CustomLookAndFeel.h    # Custom UI styling
│   ├── PluginProcessor.h          # Main audio processing
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h             # UI implementation
│   └── PluginEditor.cpp
├── Resources/
│   └── Samples/                   # Drum samples (currently synthesized)
├── BuildupFX.jucer                # Projucer project file
├── CMakeLists.txt                 # CMake build configuration
└── README.md                      # This file
```

## Development

### Adding Custom Drum Samples

Currently, the plugin uses synthesized drum samples. To add real samples:

1. Place WAV files in `Resources/Samples/`
2. In Projucer, add samples to the project as binary resources
3. Update `DrumEngine::createDefaultSamples()` to load from `BinaryData`

Example:
```cpp
auto* reader = formatManager.createReaderFor(
    std::make_unique<MemoryInputStream>(
        BinaryData::snare_01_wav,
        BinaryData::snare_01_wavSize,
        false
    )
);
```

### Customizing Intensity Curves

Edit `IntensityMapper.h` to adjust how intensity values map to parameters:

```cpp
static float getHPFCutoff(float intensity) {
    // Your custom curve here
    // Example: more aggressive sweep
    return 20.0f + std::pow(intensity / 100.0f, 2.0f) * 8000.0f;
}
```

### Debugging

Enable debug output in your DAW's plugin console:
- Set breakpoints in `processBlock()`
- Use `DBG()` macro for logging: `DBG("Buildup triggered! Length: " << selectedBuildupLength);`

## Troubleshooting

### Plugin doesn't appear in DAW
- Verify plugin is copied to correct location
- Restart DAW completely
- Check DAW's plugin scan settings
- On macOS, run: `auval -v aufx BuFx YrCo` to validate AU

### No sound output
- Check Dry/Wet mix is not at 0%
- Verify output level is not too low
- Ensure buildup is triggered (click TRIGGER button)
- Check DAW transport is playing

### Buildup doesn't sync to tempo
- Verify DAW is sending tempo/timing info to plugin
- Some DAWs require transport to be playing
- Check plugin receives PPQ position

### Clicks/pops during buildup
- Increase DAW buffer size (e.g., 512 samples)
- Plugin uses parameter smoothing, but very small buffers may still cause issues

### Build errors
- **"JUCE not found"**: Set `JUCE_DIR` correctly in CMake or Projucer global paths
- **Missing modules**: Ensure all JUCE modules are enabled in Projucer
- **C++17 errors**: Verify compiler supports C++17 standard

## License

This plugin is provided as-is for educational and personal use.

For commercial use, ensure compliance with:
- JUCE licensing (GPL v3 or commercial license)
- Any third-party sample licensing

## Credits

- Built with [JUCE Framework](https://juce.com/)
- Inspired by Pioneer RMX-1000
- Developed for house music producers

## Version History

**v1.0.0** (2024)
- Initial release
- Two-intensity buildup system
- Progressive noise and drum layering
- Multiple buildup lengths (4/8/16/32 bars)
- Four drop types
- Preset systems for noise and drums
- Auto-retrigger functionality

## Support

For issues, feature requests, or questions:
- File an issue on the GitHub repository
- Check the troubleshooting section above
- Verify you're using a compatible JUCE version (7.0+)

---

**Enjoy creating buildups! 🎵**
