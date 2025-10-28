# Compilation Guide - Null Space Jenerator

This guide will walk you through compiling NSJ step-by-step.

---

## Part 1: System Check (Do This First)

### macOS Users

**Check if Xcode Command Line Tools are installed:**

```bash
xcode-select --version
```

✅ **If you see a version number** (like `xcode-select version 2395`): You're good, skip to Part 2

❌ **If you see "command not found"**: Install it:

```bash
xcode-select --install
```

A popup will appear. Click "Install" and wait 20-30 minutes.

---

**Check if CMake is installed:**

```bash
cmake --version
```

✅ **If you see a version number** (like `cmake version 3.27.1`): You're good

❌ **If you see "command not found"**: Install with Homebrew:

```bash
# First, install Homebrew if you don't have it
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Then install CMake
brew install cmake
```

---

### Windows Users

**Check if Visual Studio is installed:**

1. Open Start Menu
2. Search for "Visual Studio"
3. If you see "Visual Studio 2019" or "Visual Studio 2022": ✅ You're good (probably)
4. If not: ❌ Download and install from https://visualstudio.microsoft.com/downloads/
   - Choose "Community" edition (free)
   - During install, select "Desktop development with C++"

---

**Check if CMake is installed:**

Open Command Prompt (cmd) and type:

```cmd
cmake --version
```

✅ **If you see a version number**: You're good

❌ **If you see "not recognized"**: Download from https://cmake.org/download/
   - Choose the Windows installer
   - During install, select "Add CMake to system PATH"
   - Restart Command Prompt after install

---

## Part 2: Download the Project

1. **Download the project folder** I gave you (should be called `null-space-jenerator`)
2. **Place it somewhere easy to find** like your Desktop or Documents folder

---

## Part 3: Open Terminal/Command Prompt

### macOS
1. Press `Cmd + Space`
2. Type "Terminal"
3. Press Enter

### Windows
1. Press `Windows Key`
2. Type "cmd"
3. Press Enter

---

## Part 4: Navigate to Project

In your terminal/command prompt, navigate to the project folder.

### macOS Example:
```bash
cd ~/Desktop/null-space-jenerator
```

### Windows Example:
```cmd
cd C:\Users\YourName\Desktop\null-space-jenerator
```

**Replace `YourName` with your actual username!**

**Verify you're in the right place:**
```bash
ls        # macOS
dir       # Windows
```

You should see: `CMakeLists.txt`, `README.md`, and `src/` folder

---

## Part 5: Create Build Directory

### Both macOS and Windows:

```bash
mkdir build
cd build
```

---

## Part 6: Configure the Build

This step downloads JUCE and sets everything up. **Takes 5-10 minutes.**

### macOS:

```bash
cmake ..
```

You'll see lots of output like:
```
-- Fetching JUCE...
-- Configuring done
-- Generating done
```

### Windows:

```cmd
cmake .. -G "Visual Studio 17 2022"
```

If you have Visual Studio 2019, use:
```cmd
cmake .. -G "Visual Studio 16 2019"
```

---

### ⚠️ Troubleshooting Configure Step

**Error: "CMake not found"**
- Solution: Go back to Part 1, install CMake

**Error: "No CMAKE_CXX_COMPILER could be found"**
- macOS: Run `xcode-select --install`
- Windows: Install Visual Studio with C++ tools

**Error: "Failed to fetch JUCE"**
- Check internet connection
- Try again (sometimes GitHub is slow)

---

## Part 7: Build the Plugin

This compiles the actual VST3. **Takes 5-10 minutes first time.**

### macOS:

```bash
cmake --build . --config Release
```

### Windows:

```cmd
cmake --build . --config Release
```

You'll see compilation progress:
```
[ 10%] Building CXX object ...
[ 20%] Building CXX object ...
...
[100%] Built target NullSpaceJenerator
```

---

### ⚠️ Troubleshooting Build Step

**Error: Compilation failed with errors**
- Read the error message carefully
- Common issue: Missing C++ compiler
  - macOS: `xcode-select --install`
  - Windows: Reinstall Visual Studio with C++ tools

**Error: Out of memory**
- Close other applications
- Try building again

**Build takes forever (30+ minutes)**
- This is normal for first build
- JUCE is a large framework
- Subsequent builds will be much faster

---

## Part 8: Install the Plugin

### macOS:

The plugin should automatically copy itself, but if not:

```bash
cp -r NullSpaceJenerator_artefacts/Release/VST3/Null\ Space\ Jenerator.vst3 \
   ~/Library/Audio/Plug-Ins/VST3/
```

**Verify it's there:**
```bash
ls ~/Library/Audio/Plug-Ins/VST3/
```

You should see `Null Space Jenerator.vst3`

---

### Windows:

**You need administrator rights for this step.**

1. **Find the compiled plugin:**
   - It's in: `build\NullSpaceJenerator_artefacts\Release\VST3\`
   - Look for `Null Space Jenerator.vst3`

2. **Copy it to the VST3 folder:**
   - Open File Explorer as Administrator (right-click, "Run as administrator")
   - Navigate to: `C:\Program Files\Common Files\VST3\`
   - Paste `Null Space Jenerator.vst3` there

---

## Part 9: Test in Ableton Live

1. **Restart Ableton Live completely** (close and reopen)

2. **Create a new track:**
   - Right-click in track area
   - Choose "Insert MIDI Track" or "Insert Instrument Track"

3. **Load the plugin:**
   - In the track, click "Instruments" browser
   - Search for "Null Space Jenerator"
   - Double-click to load

4. **Start playback:**
   - Press spacebar in Ableton
   - You should hear a sine wave sequencing!

5. **Test features:**
   - Click steps to toggle gates
   - Right-click steps to lock them
   - Click "REGENERATE" button
   - Change Ableton's BPM (should stay in sync)

---

## Success! 🎉

If you hear sound, you did it! You've successfully compiled your first VST plugin!

---

## What If It Doesn't Work?

### Plugin doesn't appear in Ableton

**macOS:**
1. Check: `~/Library/Audio/Plug-Ins/VST3/Null Space Jenerator.vst3` exists
2. In Ableton, go to Preferences → Plug-Ins
3. Check "Use VST3 Plug-in System Folders" is enabled
4. Click "Rescan"
5. Restart Ableton

**Windows:**
1. Check: `C:\Program Files\Common Files\VST3\Null Space Jenerator.vst3` exists
2. If not there, you may need admin rights to copy it
3. In Ableton, go to Preferences → Plug-Ins
4. Check VST3 folder is correct
5. Click "Rescan"
6. Restart Ableton

---

### Plugin crashes Ableton

- Try the Standalone version first:
  - **macOS**: Look in `build/NullSpaceJenerator_artefacts/Release/Standalone/`
  - **Windows**: Look in `build\NullSpaceJenerator_artefacts\Release\Standalone\`
  - Run `Null Space Jenerator.app` (macOS) or `Null Space Jenerator.exe` (Windows)
- If standalone works but VST doesn't, might be a DAW-specific issue

---

### No sound output

1. Check Ableton transport is playing (spacebar)
2. Check track is armed/monitoring
3. Check plugin UI shows white outline moving (current step indicator)
4. Check Ableton's master volume isn't muted
5. Try loading it on a fresh track

---

## Need Help?

If you're stuck, tell me:
1. **Which step failed?** (Part 1-9)
2. **What OS?** (macOS version or Windows version)
3. **Exact error message** (copy/paste the full text)

I'll help you debug! 🔧
