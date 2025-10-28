# Null Space Jenerator (NSJ)

A generative VST3 plugin for dub techno and minimal tech house production.

## Phase 1 Features

- 16-step sequencer with tempo sync
- Gate locking system (lock gates on/off to preserve during regeneration)
- Simple sine wave synthesis
- Random pattern generation
- Minimal dark UI

## System Requirements

### macOS
- macOS 11.0 or later
- Xcode 12.5 or later
- CMake 3.15 or later

### Windows
- Windows 10 or later
- Visual Studio 2019 or later (with C++ Desktop Development)
- CMake 3.15 or later

## Building from Source

### Prerequisites

Install CMake:
- **macOS**: `brew install cmake`
- **Windows**: Download from https://cmake.org/download/

### Compilation Steps

#### macOS

```bash
# 1. Navigate to project directory
cd null-space-jenerator

# 2. Create build directory
mkdir build
cd build

# 3. Configure with CMake
cmake ..

# 4. Build
cmake --build . --config Release

# 5. Install plugin
# VST3 will be automatically copied to:
# ~/Library/Audio/Plug-Ins/VST3/Null Space Jenerator.vst3

# Or manually copy:
cp -r NullSpaceJenerator_artefacts/Release/VST3/Null\ Space\ Jenerator.vst3 \
   ~/Library/Audio/Plug-Ins/VST3/
```

#### Windows

```cmd
REM 1. Navigate to project directory
cd null-space-jenerator

REM 2. Create build directory
mkdir build
cd build

REM 3. Configure with CMake (use your Visual Studio version)
cmake .. -G "Visual Studio 16 2019"

REM 4. Build
cmake --build . --config Release

REM 5. Install plugin
REM VST3 will be in: build\NullSpaceJenerator_artefacts\Release\VST3\
REM Copy to: C:\Program Files\Common Files\VST3\
```

### First Time Setup - macOS

If you don't have Xcode Command Line Tools:

```bash
xcode-select --install
```

If you don't have Homebrew (for CMake):

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install cmake
```

### First Time Setup - Windows

1. Install Visual Studio 2019 or newer
   - Download: https://visualstudio.microsoft.com/downloads/
   - Select "Desktop development with C++"
   
2. Install CMake
   - Download: https://cmake.org/download/
   - Choose "Add CMake to system PATH"

## Using the Plugin

### In Ableton Live

1. Restart Ableton Live (or rescan plugins)
2. Create a new MIDI or Instrument track
3. Load "Null Space Jenerator" from the Instruments list
4. Start playback - you should hear the sequencer!

### Controls

- **Left-click step**: Toggle gate on/off
- **Right-click step** (or Cmd+Click): Cycle through lock states
  - Gray border = Unlocked (can be randomized)
  - Green border = Locked ON (always plays)
  - Red border = Locked OFF (never plays)
- **Regenerate button**: Randomize all unlocked gates
- **Current step**: White outline shows which step is playing

### Lock States Explained

- **Unlocked**: Gate can be changed by regenerate button
- **Locked ON**: Gate always plays, immune to regeneration
- **Locked OFF**: Gate never plays, immune to regeneration

## Troubleshooting

### Plugin doesn't appear in DAW

**macOS:**
- Check it's in: `~/Library/Audio/Plug-Ins/VST3/`
- Restart DAW completely
- Check DAW plugin scan settings

**Windows:**
- Check it's in: `C:\Program Files\Common Files\VST3\`
- You may need administrator rights to copy there
- Restart DAW completely

### Build errors

**"JUCE not found":**
- CMake should automatically download JUCE
- If it fails, check your internet connection
- Try deleting `build/` folder and rebuilding

**"Cannot find compiler":**
- macOS: Run `xcode-select --install`
- Windows: Install Visual Studio with C++ tools

**"Permission denied" when copying plugin:**
- macOS: May need `sudo` for system folders
- Windows: Run as administrator

### No sound output

- Check DAW transport is playing
- Check track is armed/monitoring
- Verify plugin shows current step advancing
- Try different BPM settings

## Development Roadmap

### Phase 2 (Coming Next)
- Euclidean rhythm generation
- Multi-track sequencer (4 tracks)
- Polyphonic synthesis (8 voices)
- Basic subtractive synthesis (filter, ADSR)
- Simple delay effect

### Phase 3
- Chord builder with scale awareness
- Multiple scales (Minor, Phrygian, Dorian, etc.)
- Chord progression sequencer
- Inversion controls

### Phase 4
- Full effects chain (reverb, chorus, saturation)
- Preset system
- Enhanced UI
- MIDI output routing

## Technical Details

- **Audio Engine**: JUCE 7.x
- **Sample Rate**: Supports 44.1kHz - 192kHz
- **Buffer Sizes**: 64 - 2048 samples
- **Latency**: Near-zero (real-time safe)
- **CPU Usage**: <5% typical (single core)

## License

TBD

## Credits

Created by NSJ
Built with JUCE framework

## Support

For issues or questions, check:
- JUCE Forum: https://forum.juce.com/
- GitHub Issues: [your repo here]
