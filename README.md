# NateFTBL Music Visualizer

A web-based music visualizer application that creates stunning 3D visualizations responding to audio input in real-time.

## Features

### Audio Input Sources
- **Microphone**: Capture live audio from your device's microphone
- **Browser Tab**: Capture audio from any browser tab or application (Screen Capture API)
- **File Upload**: Play and visualize audio from uploaded files (MP3, WAV, etc.)

### Visualizer Modes

#### 1. Spectrum Bars
- 64 3D bars arranged in a circle around the text
- Each bar responds to different frequency ranges
- Dynamic color shifting based on frequency intensity
- Smooth animations and rotations

#### 2. Particle System
- 500 particles orbiting around the text
- Particles pulse and expand with bass frequencies
- Color changes respond to bass, mid, and treble levels
- Smooth orbital animations

#### 3. Color Shift
- Text colors shift dynamically based on frequency bands
- Real-time gradient effects
- Scaling and rotation effects synchronized with bass
- Glowing effects that pulse with the music

### User Interface
- **Clean Dark Theme**: Professional gradient background with glass-morphism effects
- **Custom Text Input**: Display any text you want (up to 20 characters)
- **Audio Source Selector**: Easy dropdown to switch between input sources
- **Visualizer Mode Buttons**: Quick switching between visualization modes
- **Sensitivity Slider**: Adjust visualization intensity (1-10)
- **Start/Stop Control**: Easy control of the visualizer
- **Status Indicator**: Real-time feedback on app state

## Tech Stack

- **HTML5**: Modern semantic markup
- **CSS3**: Custom styling with gradients, animations, and responsive design
- **JavaScript (ES6+)**: Modern JavaScript with modules
- **Three.js**: 3D graphics and rendering
- **Web Audio API**: Real-time audio analysis and processing
- **Screen Capture API**: Browser tab audio capture

## How to Use

### Setup

1. Open `index.html` in a modern web browser (Chrome, Firefox, Edge recommended)
2. The app requires HTTPS or localhost for microphone/tab capture features

### Using the Visualizer

1. **Enter Display Text** (optional)
   - Type your desired text in the "Display Text" field
   - Default is "NateFTBL"

2. **Select Audio Source**
   - Choose from Microphone, Browser Tab, or Upload File
   - For Browser Tab: Make sure to check "Share audio" in the system dialog

3. **Choose Visualizer Mode**
   - Click on Spectrum Bars, Particles, or Color Shift
   - You can switch modes while the visualizer is running

4. **Adjust Sensitivity**
   - Use the slider to control how much the visuals respond to audio
   - Higher values = more dramatic visualizations

5. **Start Visualizer**
   - Click "Start Visualizer" button
   - Grant necessary permissions when prompted
   - Enjoy the show!

### Tips

- **For Browser Tab Capture**: When the system dialog appears, select the tab/window you want to capture and make sure to check the "Share audio" checkbox
- **Best Performance**: Close other browser tabs and applications for smooth animations
- **Microphone Input**: Works best with music playing near the microphone or system audio output
- **File Upload**: Supports common audio formats (MP3, WAV, OGG, etc.)

## Browser Compatibility

| Feature | Chrome | Firefox | Safari | Edge |
|---------|--------|---------|--------|------|
| Microphone | ✅ | ✅ | ✅ | ✅ |
| Browser Tab Capture | ✅ | ✅ | ❌ | ✅ |
| File Upload | ✅ | ✅ | ✅ | ✅ |
| Three.js Rendering | ✅ | ✅ | ✅ | ✅ |

**Note**: Safari does not support Screen Capture API for tab audio capture.

## File Structure

```
NSJ/
├── index.html          # Main HTML structure
├── styles.css          # Styling and themes
├── app.js              # Main application controller
├── audioManager.js     # Audio input and analysis
├── visualizers.js      # Three.js visualizations
└── README.md           # Documentation
```

## Architecture

### AudioManager (`audioManager.js`)
- Handles audio context initialization
- Manages different audio input sources
- Provides FFT analysis data
- Extracts bass, mid, and treble frequency levels

### VisualizerEngine (`visualizers.js`)
- Three.js scene setup and rendering
- 3D text rendering using canvas textures
- Three distinct visualization modes
- Camera and lighting management

### MusicVisualizerApp (`app.js`)
- Main application controller
- UI event handling
- Animation loop coordination
- State management

## API Reference

### AudioManager Methods

```javascript
// Initialize audio context
await audioManager.init()

// Connect to microphone
await audioManager.connectMicrophone()

// Connect to browser tab
await audioManager.connectTabAudio()

// Connect to audio file
await audioManager.connectFile(file)

// Get frequency data
const freqData = audioManager.getFrequencyData()

// Get bass/mid/treble levels
const bass = audioManager.getBass()
const mid = audioManager.getMid()
const treble = audioManager.getTreble()

// Set sensitivity
audioManager.setSensitivity(5.0)
```

### VisualizerEngine Methods

```javascript
// Set visualization mode
visualizer.setMode('spectrum' | 'particles' | 'colorshift')

// Update display text
visualizer.updateText('Your Text')

// Update and render
visualizer.update(audioManager)
visualizer.render()
```

## Customization

### Modifying Visualizations

All visualizer code is in `visualizers.js`. You can customize:
- Number of spectrum bars (default: 64)
- Number of particles (default: 500)
- Colors and materials
- Animation speeds
- Camera positions

### Styling

All styles are in `styles.css`. You can customize:
- Color schemes
- Button styles
- Layout and spacing
- Responsive breakpoints

## Performance Optimization

- FFT size: 2048 (balance between resolution and performance)
- 60 FPS target for smooth animations
- Efficient particle system using BufferGeometry
- Conditional rendering based on active visualizer mode

## Known Limitations

1. Safari does not support Screen Capture API
2. Audio file playback loops automatically
3. Maximum text length: 20 characters
4. Requires HTTPS for microphone/tab capture (except on localhost)

## Future Enhancements

- Additional visualizer modes
- Recording functionality
- Preset configurations
- Fullscreen mode
- VR support
- More audio effects

## License

This project is open source and available for educational purposes.

## Credits

Created with Three.js and Web Audio API
Developed for NateFTBL

## Support

For issues or questions, please check browser console for error messages.

---

**Enjoy the music and visuals!** 🎵✨
