# BuildupFX - Quick Start Guide

## 🚀 Get Building in 5 Minutes

### Prerequisites
- JUCE Framework (download from https://juce.com/get-juce/)
- Xcode (macOS), Visual Studio 2022 (Windows), or GCC/Clang (Linux)

### Build Steps

#### Option 1: Projucer (Easiest)
```bash
1. Open Projucer
2. File → Open → Select BuildupFX.jucer
3. Click "Save and Open in IDE"
4. Build in your IDE (⌘+B on Mac, F7 on Windows)
5. Plugin will be in: Builds/[Platform]/build/Release/
```

#### Option 2: CMake
```bash
cd BuildupFX
mkdir build && cd build
cmake -DJUCE_DIR=/path/to/JUCE -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Installation
Copy the built plugin to:
- **macOS VST3**: `~/Library/Audio/Plug-Ins/VST3/`
- **macOS AU**: `~/Library/Audio/Plug-Ins/Components/`
- **Windows VST3**: `C:\Program Files\Common Files\VST3\`
- **Linux VST3**: `~/.vst3/`

### First Use
1. Load BuildupFX on your master channel in your DAW
2. Set buildup length (16 bars recommended)
3. Adjust Noise Intensity (try 65%)
4. Adjust Drum Intensity (try 80%)
5. Click **TRIGGER** button 16 bars before your drop
6. Watch the magic happen! ✨

### Need Help?
See full README.md for detailed documentation.

---

**🎵 Happy Producing!**
