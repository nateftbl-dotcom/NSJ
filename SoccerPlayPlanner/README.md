# Soccer Play Planner

A mobile iPhone app for planning and visualizing soccer tactical plays using a keyframe-based animation system.

## Features

### Core Functionality
- **Keyframe Animation System**: Create step-by-step plays by setting player positions at key moments
- **Smooth Interpolation**: Players automatically move smoothly between keyframes with easing
- **Formation Presets**: Quick setup with 7 common formations (4-3-3, 4-4-2, 3-5-2, 4-2-3-1, 5-3-2, 3-4-3, 4-5-1)
- **Custom Formations**: Save and reuse your own formations
- **Dual Teams**: Set up both home and away teams with distinct colors

### Playback Controls
- Play/Pause animation
- Adjustable playback speed (0.25x to 2x)
- Loop mode for continuous playback
- Scrub through timeline
- Visual timeline with keyframe markers

### Export & Sharing
- **Video Export**: Generate MP4 videos of your plays
- **PDF Export**: Create printable diagrams with all keyframes
- **Project Files**: Share editable .soccerplay files
- iOS Share Sheet integration

### User Interface
- Professional soccer field with proper markings
- Draggable player pieces with numbers
- Color-coded teams (Blue = Home, Red = Away)
- Timeline with visual keyframe markers
- Formation picker
- Project management

## Project Structure

```
SoccerPlayPlanner/
├── Models/
│   ├── Player.swift              # Player data model
│   ├── Formation.swift            # Formation presets and custom formations
│   ├── Keyframe.swift             # Keyframe data model
│   └── Project.swift              # Project container
├── Views/
│   ├── ContentView.swift          # Main navigation
│   ├── MainEditorView.swift       # Main editing interface
│   ├── FieldView.swift            # Soccer field rendering
│   ├── PlayerView.swift           # Draggable player pieces
│   ├── TimelineView.swift         # Timeline with keyframes
│   ├── PlaybackControlsView.swift # Playback controls
│   ├── FormationPickerView.swift  # Formation selection
│   ├── ProjectListView.swift      # Project list
│   ├── NewProjectView.swift       # New project creation
│   └── ExportOptionsView.swift    # Export and sharing
├── ViewModels/
│   ├── EditorViewModel.swift      # Editor state management
│   └── ProjectManager.swift       # Project persistence
├── Utils/
│   ├── AnimationInterpolator.swift # Keyframe interpolation
│   ├── VideoExporter.swift         # Video export functionality
│   └── PDFGenerator.swift          # PDF export functionality
└── SoccerPlayPlannerApp.swift     # App entry point
```

## How It Works

### Keyframe System
The app uses a keyframe-based animation system similar to video editing software:

1. **Set Initial Formation**: Start with preset formations for both teams
2. **Add Keyframes**: Drag players to new positions and tap "Add Keyframe"
3. **Build the Play**: Continue adding keyframes at different times
4. **Smooth Animation**: The app automatically interpolates movement between keyframes

### Animation Interpolation
- Uses cubic easing (ease-in-out) for natural movement
- Interpolates position for each player between adjacent keyframes
- Maintains smooth 60 FPS playback

### Data Persistence
- Projects saved as JSON files in app documents directory
- Auto-saves on every change
- Export projects as `.soccerplay` files for sharing

## Setup Instructions

### Requirements
- Xcode 15.0 or later
- iOS 17.0 or later
- macOS Sonoma or later

### Installation

1. **Open in Xcode**:
   ```bash
   cd SoccerPlayPlanner
   open SoccerPlayPlanner.xcodeproj
   ```

2. **Configure Bundle Identifier**:
   - Select the project in Xcode
   - Under "Signing & Capabilities", set your Team
   - Update the Bundle Identifier if needed (e.g., `com.yourname.SoccerPlayPlanner`)

3. **Build and Run**:
   - Select your target device or simulator
   - Press Cmd+R to build and run

### Creating an Xcode Project (if needed)

If you need to create the Xcode project from scratch:

1. Open Xcode
2. Create a new project:
   - Choose "iOS" → "App"
   - Product Name: `SoccerPlayPlanner`
   - Interface: SwiftUI
   - Language: Swift
   - Organization Identifier: `com.yourname`
3. Add all the source files to the project
4. Ensure all files are in the correct groups:
   - Models folder → Models group
   - Views folder → Views group
   - ViewModels folder → ViewModels group
   - Utils folder → Utils group

## Usage

### Creating a New Project
1. Tap the "+" button
2. Enter a project name
3. Select formations for home and away teams
4. Tap "Create"

### Editing a Play
1. Tap a project to open the editor
2. Drag players to desired positions
3. Tap "Add Keyframe" to save the current state
4. Use the timeline to navigate between keyframes
5. Repeat to build your play

### Playback
- Tap the play button to watch the animation
- Use the slider to scrub through the timeline
- Adjust speed with the speed menu
- Enable loop mode for continuous playback

### Exporting
1. Tap the export button
2. Choose export format:
   - Video: Creates MP4 video
   - PDF: Creates printable diagrams
   - Project File: Creates shareable .soccerplay file
3. Use iOS Share Sheet to save or share

## Technical Details

### Key Technologies
- **SwiftUI**: Modern declarative UI framework
- **Combine**: Reactive programming for state management
- **AVFoundation**: Video export functionality
- **PDFKit**: PDF generation
- **CoreGraphics**: Custom field rendering

### Performance Considerations
- 60 FPS playback using Timer
- Efficient position interpolation
- Normalized coordinates (0-1) for resolution independence
- Lazy loading of projects

### File Formats
- **Project Files**: JSON format with `.soccerplay` extension
- **Video Export**: H.264 MP4 (1920x1080, 60fps)
- **PDF Export**: US Letter size with field diagrams

## Future Enhancements

Potential features for future versions:
- Ball movement visualization
- Pass lines and arrows
- Defensive pressure indicators
- Player roles and labels
- Animation paths (curved movements)
- Cloud sync (iCloud)
- iPad optimization
- Playbook organization
- Team/player templates
- GIF export
- Slow-motion highlights

## License

This project is open source and available for educational purposes.

## Author

Created for soccer coaches, players, and tactical enthusiasts who want a professional tool for planning and sharing plays.
