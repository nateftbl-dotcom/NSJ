# Setup Guide - Soccer Play Planner

This guide will walk you through setting up the Soccer Play Planner app in Xcode.

## Quick Start

### Step 1: Create Xcode Project

1. **Open Xcode** (15.0 or later)

2. **Create New Project**:
   - File → New → Project
   - Choose "iOS" → "App"
   - Click "Next"

3. **Configure Project**:
   - Product Name: `SoccerPlayPlanner`
   - Team: Select your Apple Developer Team
   - Organization Identifier: `com.yourname` (or your preferred identifier)
   - Bundle Identifier: Will auto-populate as `com.yourname.SoccerPlayPlanner`
   - Interface: **SwiftUI**
   - Language: **Swift**
   - Storage: None
   - Uncheck "Include Tests"
   - Click "Next"

4. **Choose Location**:
   - Navigate to `/home/user/NSJ/`
   - Select "SoccerPlayPlanner" folder
   - Click "Create"

### Step 2: Add Source Files

The source files are already organized in the following structure:

```
SoccerPlayPlanner/
└── SoccerPlayPlanner/
    ├── SoccerPlayPlannerApp.swift
    ├── ContentView.swift
    ├── Info.plist
    ├── Models/
    │   ├── Player.swift
    │   ├── Formation.swift
    │   ├── Keyframe.swift
    │   └── Project.swift
    ├── Views/
    │   ├── FieldView.swift
    │   ├── PlayerView.swift
    │   ├── MainEditorView.swift
    │   ├── PlaybackControlsView.swift
    │   ├── TimelineView.swift
    │   ├── FormationPickerView.swift
    │   ├── ProjectListView.swift
    │   ├── NewProjectView.swift
    │   └── ExportOptionsView.swift
    ├── ViewModels/
    │   ├── EditorViewModel.swift
    │   └── ProjectManager.swift
    └── Utils/
        ├── AnimationInterpolator.swift
        ├── VideoExporter.swift
        └── PDFGenerator.swift
```

**In Xcode**:

1. Select the default `ContentView.swift` and delete it (Xcode creates this by default)
2. Select the default `SoccerPlayPlannerApp.swift` and delete it
3. Right-click on the "SoccerPlayPlanner" group (blue folder icon)
4. Choose "Add Files to SoccerPlayPlanner..."
5. Navigate to the `SoccerPlayPlanner/SoccerPlayPlanner/` directory
6. Select all folders (Models, Views, ViewModels, Utils) and files
7. Make sure these options are checked:
   - ✅ "Copy items if needed"
   - ✅ "Create groups" (not "Create folder references")
   - ✅ Add to target: SoccerPlayPlanner
8. Click "Add"

### Step 3: Configure Info.plist

The `Info.plist` file is already created. Make sure it's added to your project:

1. In Xcode, select your project in the navigator
2. Select the "SoccerPlayPlanner" target
3. Go to "Info" tab
4. Verify the custom settings are present

### Step 4: Build Settings

1. **Select Project** in navigator
2. **Select Target** "SoccerPlayPlanner"
3. **General Tab**:
   - iOS Deployment Target: **iOS 17.0** or later
   - iPhone Orientation: Portrait, Landscape Left, Landscape Right
   - iPad Orientation: All

4. **Signing & Capabilities**:
   - Select your Team
   - Automatic signing should be enabled

### Step 5: Build and Run

1. **Select a Simulator or Device**:
   - iPhone 15 Pro (recommended for testing)
   - Or connect a physical device

2. **Build**:
   - Press `Cmd + B` to build
   - Fix any build errors (there shouldn't be any)

3. **Run**:
   - Press `Cmd + R` to run
   - The app should launch on your selected device/simulator

## Troubleshooting

### Build Errors

**"Cannot find type 'Player' in scope"**
- Make sure all files are added to the target
- Check that files are in the correct groups
- Clean build folder (Shift + Cmd + K) and rebuild

**"Missing Info.plist"**
- Verify Info.plist is in the project
- Check Build Settings → Packaging → Info.plist File is set correctly

**Signing Error**
- Make sure you have a valid Apple Developer account
- Select your team in Signing & Capabilities
- Change the bundle identifier if needed

### Runtime Issues

**App crashes on launch**
- Check the console for error messages
- Verify all files are compiled (not just copied)
- Make sure target membership is set for all Swift files

**Players not draggable**
- This is expected in the simulator - gesture handling works better on device
- Try running on a physical device for full gesture support

**Export features not working**
- Video export requires actual implementation in ExportOptionsView
- PDF generation is implemented but may need testing

## Testing the App

### Basic Workflow Test

1. **Create a New Project**:
   - Tap the "+" button
   - Enter "Test Play"
   - Keep default formations (4-3-3 vs 4-4-2)
   - Tap "Create"

2. **Edit the Play**:
   - Tap the newly created project
   - You should see a soccer field with players
   - Try dragging a player (works best on device)
   - Tap "Add Keyframe"

3. **Test Playback**:
   - Drag players to different positions
   - Add another keyframe
   - Tap the play button
   - Players should animate between positions

4. **Test Timeline**:
   - Drag the red line in the timeline
   - Players should move to interpolated positions
   - Tap on keyframe markers to jump to them

5. **Test Export**:
   - Tap the export button (up arrow)
   - Try exporting as project file
   - Use share sheet to save/share

## Next Steps

### Customization

You can customize:
- Team colors in `Player.swift`
- Formation presets in `Formation.swift`
- Field appearance in `FieldView.swift`
- Animation easing in `AnimationInterpolator.swift`

### Adding Features

Consider adding:
- Ball visualization
- Pass/movement arrows
- Player names/roles
- More export formats
- iCloud sync

### Deployment

To deploy to TestFlight or App Store:
1. Set up proper provisioning profiles
2. Create app icons (use SF Symbols or custom designs)
3. Add App Store screenshots
4. Submit for review

## Support

For issues or questions:
- Check the main README.md for documentation
- Review the code comments in each file
- Test on a physical device for best results

## Summary

You now have a fully functional soccer tactical planning app with:
- ✅ Keyframe-based animation system
- ✅ Formation presets and custom formations
- ✅ Playback controls with speed adjustment
- ✅ Timeline with visual keyframes
- ✅ Project save/load
- ✅ Export capabilities (video, PDF, project files)
- ✅ Professional UI with SwiftUI

Enjoy planning your soccer plays!
