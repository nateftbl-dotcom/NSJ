# Dub Techno Chord Generator - Web Version

A browser-based dub techno chord generator built with the Web Audio API. No samples required - pure synthesis.

## Features

### Synthesis Engine
- **Multiple Waveforms**: Sine, Triangle, Sawtooth, Square
- **Voice Stacking**: 3 oscillators per note with detune for thick, chorus-like sound
- **Envelope Control**: Adjustable attack and release for smooth transitions
- **Detune Control**: Create subtle pitch variations for analog warmth

### Chord Types
- Minor 7th (classic dub techno)
- Minor 9th (extended harmony)
- Sus2 (open, atmospheric)
- Sus4 (tension and release)
- Major 7th (brighter mood)
- Dominant 7th (traditional jazz flavor)

### Filter Section
- **Filter Types**: Low-pass, High-pass, Band-pass
- **Cutoff Frequency**: 100Hz - 8000Hz
- **Resonance**: 0 - 30 (emphasize frequencies at cutoff)

### Effects Chain
- **Delay**:
  - Delay Time: 0.1s - 2s
  - Feedback: 0 - 0.8
  - Wet/Dry Mix: 0 - 1
- **Reverb**:
  - 3-second impulse response
  - Wet/Dry Mix: 0 - 1
- **Master Volume**: 0 - 0.5

## How to Use

1. **Open** `dub-techno-generator.html` in any modern web browser (Chrome, Firefox, Safari)
2. **Click** "GENERATE NEW CHORD" to create a random chord
3. **Click** "PLAY CHORD" to hear the generated chord
4. **Adjust** controls in real-time while the chord plays
5. **Click** "STOP" to end playback with smooth envelope release
6. **Generate** different chords until you find one you like, then play it again
7. **Experiment** with different waveforms, filters, and effects to craft your signature sound

### Workflow
- **Generate** creates a new random chord but doesn't play it
- **Play** plays the current chord (you can play the same chord multiple times)
- **Stop** ends playback gracefully
- This separation lets you preview chord options before committing to one

## Sound Design Tips

### Classic Dub Techno
- Waveform: Sawtooth
- Chord Type: Minor 7th
- Filter: Low-pass, 400-800Hz cutoff, moderate resonance
- Delay: 0.375s (dotted 8th at 120 BPM), feedback 0.4-0.6
- Reverb Mix: 0.3-0.5

### Deep Atmospheric
- Waveform: Sine or Triangle
- Chord Type: Minor 9th or Sus2
- Filter: Low-pass, 300-600Hz
- Delay: Longer time (0.5-1s), lower feedback
- Reverb Mix: 0.5-0.7
- Detune: 20-30 for more movement

### Minimal Tech
- Waveform: Square
- Chord Type: Sus4 or Dominant 7th
- Filter: Band-pass, 800-2000Hz
- Delay: Short (0.2-0.3s), medium feedback
- Reverb Mix: 0.2-0.3
- Detune: 5-10 for tight sound

## Technical Details

### Audio Architecture
```
Oscillators (x3 per note)
    ↓
Individual Gain Envelopes
    ↓
Biquad Filter
    ↓
Delay (with feedback loop) + Dry Signal
    ↓
Convolver Reverb + Dry Signal
    ↓
Master Gain
    ↓
Output
```

### Browser Compatibility
- Chrome/Edge: Full support ✓
- Firefox: Full support ✓
- Safari (Desktop): Full support ✓
- Safari (iOS/iPhone): Full support ✓ (requires tapping Generate first)
- Requires modern browser with Web Audio API support

### iOS/iPhone Instructions
1. Open the HTML file in Safari
2. Tap "GENERATE NEW CHORD" - this activates the audio context
3. Tap "PLAY CHORD" - you should now hear sound
4. If no sound, check that your phone isn't on silent mode
5. Increase volume if needed

The first tap on any button will activate audio on iOS devices.

## Performance Notes

- Low CPU usage (pure Web Audio API nodes)
- No external dependencies
- Works offline once loaded
- Responsive design for mobile and desktop

## Integration with VST Plugin

This web generator complements the Null Space Jenerator VST3 plugin:
- **Web**: Quick experimentation and sound design
- **VST**: Full DAW integration with sequencer and MIDI

Both share the dub techno aesthetic but serve different workflows.

## Future Enhancements

Potential additions:
- Arpeggiator mode
- Pattern sequencer
- Preset system (localStorage)
- MIDI output support
- Recording/export functionality
- Additional scale modes (Phrygian, Dorian, etc.)

## License

TBD

## Credits

Created as part of the Null Space Jenerator project
Built with Web Audio API
