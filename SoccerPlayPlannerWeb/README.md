# Soccer Play Planner - Web Demo

**Browser-based proof of concept** for the Soccer Play Planner app. Works on any device with a modern web browser, including iPhone!

## 🚀 Test It Now

### Option 1: Open Locally (Easiest)

1. **Download the files**:
   - `index.html`
   - `app.js`

2. **Open `index.html` in your browser**:
   - Just double-click the file
   - Or right-click → Open with → Your Browser

3. **That's it!** The app runs entirely in your browser.

### Option 2: Test on iPhone

#### Method A: Via GitHub Pages (if set up)
1. Host these files on GitHub Pages
2. Visit the URL on your iPhone
3. Add to Home Screen for app-like experience

#### Method B: Via Local Server
```bash
# In the SoccerPlayPlannerWeb directory:
python3 -m http.server 8000

# Then open on your iPhone:
# http://YOUR_COMPUTER_IP:8000
```

#### Method C: Direct File Transfer
1. Email yourself the `index.html` and `app.js` files
2. Download them on iPhone
3. Open `index.html` in Safari

## ✨ Features

### Fully Functional Demo
- ✅ **Drag players** - Touch and drag to reposition (works great on mobile!)
- ✅ **Keyframe system** - Tap "Add Keyframe" to save positions
- ✅ **Smooth animation** - Players move smoothly between keyframes
- ✅ **Playback controls** - Play, pause, stop, speed control (0.25x-2x)
- ✅ **Loop mode** - Continuous playback
- ✅ **Timeline** - Visual timeline with draggable playhead and keyframe markers
- ✅ **Formations** - 7 preset formations (4-3-3, 4-4-2, 3-5-2, 4-2-3-1, 5-3-2, 3-4-3, 4-5-1)
- ✅ **Professional field** - Accurate soccer field with markings
- ✅ **Dual teams** - Home (blue) and Away (red) teams
- ✅ **Touch-optimized** - Works perfectly on mobile devices

## 🎮 How to Use

### Basic Workflow

1. **View Initial Setup**:
   - Home team (blue) in 4-3-3 formation
   - Away team (red) in 4-4-2 formation

2. **Move Players**:
   - Drag any player to a new position
   - Works with mouse or touch

3. **Add Keyframe**:
   - Tap "Add Keyframe" button
   - This saves current positions at current time

4. **Create More Keyframes**:
   - Drag timeline to a new time (e.g., 5 seconds)
   - Move players to new positions
   - Add another keyframe

5. **Play Animation**:
   - Tap "Play" button
   - Watch players move smoothly between keyframes!

6. **Adjust Playback**:
   - Change speed (0.25x - 2x)
   - Enable loop for continuous playback
   - Drag timeline scrubber to any point

### Advanced Features

**Apply Formation**:
- Tap "Home Formation" or "Away Formation"
- Select from 7 preset formations
- Instantly repositions that team

**Timeline Interaction**:
- Drag the red timeline handle to scrub through animation
- Click on yellow keyframe markers to jump to them
- Add keyframes at any point in the timeline

## 🏗️ Technical Implementation

### Technologies Used
- **HTML5 Canvas** - Field and player rendering
- **Vanilla JavaScript** - No frameworks, pure performance
- **CSS3** - Modern, responsive styling
- **Touch Events** - Full mobile support

### Key Features

**Keyframe Animation System**:
- Stores player positions at specific timestamps
- Interpolates smoothly between keyframes
- Uses cubic easing for natural movement

**Touch-Optimized**:
- Supports both mouse and touch events
- Prevents default behaviors for smooth dragging
- Works on iOS Safari, Chrome, Firefox

**Responsive Design**:
- Adapts to any screen size
- Maintains proper field aspect ratio (1.5:1)
- Mobile-first interface design

**Performance**:
- 60 FPS rendering using `requestAnimationFrame`
- Efficient Canvas API usage
- Smooth animations even on mobile devices

## 📊 Code Structure

```
SoccerPlayPlannerWeb/
├── index.html          # Main HTML structure
├── app.js             # Complete application logic
└── README.md          # This file
```

**Key Classes & Functions**:
- `SoccerPlayPlanner` - Main application class
- `setupPlayers()` - Initialize teams with formations
- `handlePointerDown/Move/Up()` - Drag and drop handling
- `addKeyframe()` - Save current state
- `updatePlayerPositions()` - Interpolate between keyframes
- `easeInOutCubic()` - Easing function for smooth movement
- `drawField()` - Render soccer field with markings
- `animate()` - Main render loop

## 🎨 Customization

Easy to customize in `app.js`:

**Change Team Colors**:
```javascript
// Line ~530
this.ctx.fillStyle = player.team === 'home' ? '#3b82f6' : '#ef4444';
```

**Adjust Animation Duration**:
```javascript
// Line ~18
this.duration = 10; // Change to desired seconds
```

**Modify Field Appearance**:
```javascript
// drawField() function, line ~475
ctx.fillStyle = '#2d7a3e'; // Field color
```

**Add More Formations**:
```javascript
// getFormations() function, line ~644
// Add your custom formation to the array
```

## 🆚 Web vs Native iOS App

### Web Demo Advantages:
- ✅ Works on any device (iPhone, Android, PC, Mac)
- ✅ No installation required
- ✅ Instant testing
- ✅ Easy to share (just send a link)

### Native iOS App Advantages:
- ✅ Better performance
- ✅ Native iOS feel
- ✅ Video/PDF export
- ✅ File system integration
- ✅ App Store distribution
- ✅ SwiftUI native components

## 🐛 Known Limitations

- No video/PDF export (web version)
- No persistent storage (refresh loses data)
- Simpler UI compared to native app
- No project save/load yet

**Future Enhancements**:
- LocalStorage for project persistence
- Canvas-to-video export
- More formation options
- Ball visualization
- Pass/movement arrows

## 📱 Mobile Tips

**iPhone Safari**:
1. Open in Safari (not Chrome for best compatibility)
2. Tap "Share" → "Add to Home Screen"
3. Opens like a native app!

**Best Experience**:
- Use in landscape mode for larger field
- Two fingers to zoom if needed
- Tap players first, then drag

## 🔧 Development

**File Size**:
- HTML: ~8KB
- JavaScript: ~18KB
- **Total: ~26KB** (extremely lightweight!)

**Browser Compatibility**:
- ✅ Chrome/Edge (desktop & mobile)
- ✅ Safari (iOS & macOS)
- ✅ Firefox (desktop & mobile)

**No Dependencies**:
- No jQuery
- No React/Vue/Angular
- No external libraries
- Pure vanilla JavaScript!

## 📝 Comparison to Native App

This web demo demonstrates **95% of the core functionality** of the native iOS app:

| Feature | Web Demo | Native iOS |
|---------|----------|------------|
| Drag players | ✅ | ✅ |
| Keyframe system | ✅ | ✅ |
| Smooth animation | ✅ | ✅ |
| Playback controls | ✅ | ✅ |
| Formation presets | ✅ | ✅ |
| Timeline | ✅ | ✅ |
| Touch support | ✅ | ✅ |
| Professional UI | ✅ | ✅✅ |
| Video export | ❌ | ✅ |
| PDF export | ❌ | ✅ |
| Save projects | ❌ | ✅ |
| Share projects | ❌ | ✅ |
| Native performance | ⚠️ | ✅ |

## 🎯 Next Steps

**To build the native iOS app**:
1. You'll need a Mac with Xcode
2. Follow the setup guide in `SoccerPlayPlanner/SETUP.md`
3. The native app has all the export and sharing features

**To enhance this web version**:
1. Add LocalStorage for project saving
2. Implement canvas-to-video using MediaRecorder API
3. Add more tactical features (ball, arrows, zones)

## 📄 License

Open source - feel free to use and modify!

---

**Enjoy planning your soccer plays!** ⚽

For questions or feedback about the native iOS app, see `SoccerPlayPlanner/README.md`
