# QUICK REFERENCE - Null Space Jenerator Compilation

## ⚡ Fast Track Commands

### macOS - Copy/Paste These (in order):

```bash
# 1. Go to project folder (adjust path!)
cd ~/Desktop/null-space-jenerator

# 2. Create build directory
mkdir build && cd build

# 3. Configure (downloads JUCE - takes a few mins)
cmake ..

# 4. Build (takes 5-10 mins)
cmake --build . --config Release

# 5. Install plugin (if not auto-copied)
cp -r NullSpaceJenerator_artefacts/Release/VST3/Null\ Space\ Jenerator.vst3 \
   ~/Library/Audio/Plug-Ins/VST3/

# 6. Done! Restart Ableton and load plugin
```

---

### Windows - Copy/Paste These (in Command Prompt):

```cmd
REM 1. Go to project folder (adjust path!)
cd C:\Users\YourName\Desktop\null-space-jenerator

REM 2. Create build directory
mkdir build
cd build

REM 3. Configure (downloads JUCE - takes a few mins)
cmake .. -G "Visual Studio 17 2022"

REM 4. Build (takes 5-10 mins)
cmake --build . --config Release

REM 5. Copy plugin to VST3 folder (need admin rights)
REM Manual: Copy from build\NullSpaceJenerator_artefacts\Release\VST3\
REM      to C:\Program Files\Common Files\VST3\

REM 6. Done! Restart Ableton and load plugin
```

---

## 🔧 Prerequisites Check

### macOS:
```bash
# Check Xcode Command Line Tools
xcode-select --version

# If missing, install:
xcode-select --install

# Check CMake
cmake --version

# If missing, install:
brew install cmake
```

### Windows:
```cmd
REM Check CMake
cmake --version

REM Check Visual Studio (should see it in Start Menu)
REM If missing: Download from visualstudio.microsoft.com
```

---

## 📁 Project Structure

```
null-space-jenerator/
├── CMakeLists.txt          # Build configuration
├── README.md               # Full documentation
├── COMPILE_GUIDE.md        # Detailed step-by-step
├── src/
│   ├── PluginProcessor.h   # JUCE plugin wrapper
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h      # UI
│   ├── PluginEditor.cpp
│   ├── SimpleSequencer.h   # Sequencer logic
│   ├── SimpleSequencer.cpp
│   ├── SimpleSynth.h       # Sine synth
│   └── SimpleSynth.cpp
└── build/                  # Created by you (build files go here)
```

---

## 🎹 Using the Plugin

### Controls:
- **Left-click step**: Toggle gate on/off
- **Right-click step**: Cycle lock state (unlocked → locked on → locked off)
- **REGENERATE button**: Randomize unlocked gates

### Lock States:
- **Gray border**: Unlocked (can be randomized)
- **Green border**: Locked ON (always plays)
- **Red border**: Locked OFF (never plays)

### Visual Feedback:
- **Blue step**: Gate is active
- **Dark step**: Gate is off
- **White outline**: Current playing step

---

## ⚠️ Common Issues

### "Plugin doesn't appear in Ableton"
✅ **Solution**: 
1. Restart Ableton completely
2. Check Preferences → Plug-Ins → Rescan
3. Verify VST3 is in correct folder (see install commands above)

### "No sound"
✅ **Solution**:
1. Check Ableton transport is playing (spacebar)
2. Verify white outline is moving in plugin (shows current step)
3. Check at least some gates are blue (active)

### "Build failed"
✅ **Solution**:
1. Read error message carefully
2. Make sure Xcode/Visual Studio is installed
3. Delete `build/` folder and try again
4. Check internet connection (CMake downloads JUCE)

---

## 📊 Build Times (Typical)

| Step | First Time | Subsequent |
|------|-----------|------------|
| Configure | 5-10 min | 5 sec |
| Build | 5-10 min | 30 sec |
| **Total** | **10-20 min** | **1 min** |

*First build downloads JUCE (~500MB) and compiles everything*

---

## 🎯 Quick Test Checklist

After loading in Ableton:

- [ ] Plugin window appears
- [ ] White outline moves with playback
- [ ] Left-clicking steps toggles blue/gray
- [ ] Right-clicking steps changes border color
- [ ] Sound plays when blue steps are hit
- [ ] REGENERATE button changes pattern
- [ ] Locked gates don't change on regenerate
- [ ] Plugin syncs to Ableton's BPM

---

## 💾 Plugin Locations

**macOS VST3:**
```
~/Library/Audio/Plug-Ins/VST3/Null Space Jenerator.vst3
```

**macOS AU:**
```
~/Library/Audio/Plug-Ins/Components/Null Space Jenerator.component
```

**Windows VST3:**
```
C:\Program Files\Common Files\VST3\Null Space Jenerator.vst3
```

---

## 🆘 Need Help?

If stuck, provide:
1. Operating system (macOS 14.2, Windows 11, etc.)
2. Which command failed
3. Full error message (copy/paste)

Read COMPILE_GUIDE.md for detailed troubleshooting!
