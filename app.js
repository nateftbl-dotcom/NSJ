/**
 * Main Application Controller
 */
import { AudioManager } from './audioManager.js';
import { VisualizerEngine } from './visualizers.js';

class MusicVisualizerApp {
    constructor() {
        this.audioManager = null;
        this.visualizer = null;
        this.isRunning = false;
        this.animationId = null;
        this.currentSource = 'microphone';

        // Wait for Three.js to load
        if (typeof THREE === 'undefined') {
            console.log('Waiting for Three.js to load...');
            window.addEventListener('load', () => {
                this.init();
            });
        } else {
            this.init();
        }
    }

    /**
     * Initialize the app
     */
    init() {
        console.log('Initializing Music Visualizer App...');
        this.initializeElements();
        this.setupEventListeners();
        console.log('Music Visualizer App ready!');
    }

    /**
     * Initialize DOM element references
     */
    initializeElements() {
        // Controls
        this.textInput = document.getElementById('textInput');
        this.audioSourceSelect = document.getElementById('audioSource');
        this.fileInput = document.getElementById('fileInput');
        this.sensitivitySlider = document.getElementById('sensitivity');
        this.sensitivityValue = document.getElementById('sensitivityValue');
        this.startStopBtn = document.getElementById('startStopBtn');
        this.statusElement = document.getElementById('status');
        this.playIcon = document.querySelector('.play-icon');
        this.stopIcon = document.querySelector('.stop-icon');
        this.btnText = document.querySelector('.btn-text');

        // Mode buttons
        this.modeButtons = document.querySelectorAll('.mode-btn');

        // Canvas container
        this.canvasContainer = document.getElementById('canvasContainer');

        console.log('DOM elements initialized');
    }

    /**
     * Setup event listeners
     */
    setupEventListeners() {
        // Text input
        this.textInput.addEventListener('input', (e) => {
            if (this.visualizer) {
                this.visualizer.updateText(e.target.value);
            }
        });

        // Audio source selector
        this.audioSourceSelect.addEventListener('change', (e) => {
            this.currentSource = e.target.value;

            // Show file input if file source selected
            if (this.currentSource === 'file') {
                this.fileInput.style.display = 'inline-block';
                this.fileInput.click();
            } else {
                this.fileInput.style.display = 'none';
            }

            // Restart if already running
            if (this.isRunning) {
                this.stop();
                setTimeout(() => this.start(), 100);
            }
        });

        // File input
        this.fileInput.addEventListener('change', async (e) => {
            if (e.target.files.length > 0) {
                const file = e.target.files[0];
                if (this.isRunning) {
                    await this.connectAudioSource(file);
                }
            }
        });

        // Sensitivity slider
        this.sensitivitySlider.addEventListener('input', (e) => {
            const value = parseFloat(e.target.value);
            this.sensitivityValue.textContent = value.toFixed(1);
            if (this.audioManager) {
                this.audioManager.setSensitivity(value);
            }
        });

        // Mode buttons
        this.modeButtons.forEach(btn => {
            btn.addEventListener('click', (e) => {
                // Update active state
                this.modeButtons.forEach(b => b.classList.remove('active'));
                btn.classList.add('active');

                // Set visualizer mode
                const mode = btn.dataset.mode;
                if (this.visualizer) {
                    this.visualizer.setMode(mode);
                }
            });
        });

        // Start/Stop button
        this.startStopBtn.addEventListener('click', () => {
            if (this.isRunning) {
                this.stop();
            } else {
                this.start();
            }
        });
    }

    /**
     * Start the visualizer
     */
    async start() {
        try {
            console.log('Starting visualizer...');
            this.updateStatus('Initializing...', 'active');

            // Initialize audio manager
            if (!this.audioManager) {
                console.log('Creating AudioManager...');
                this.audioManager = new AudioManager();
                const initialized = await this.audioManager.init();

                if (!initialized) {
                    throw new Error('Failed to initialize audio context');
                }

                // Set initial sensitivity
                const sensitivity = parseFloat(this.sensitivitySlider.value);
                this.audioManager.setSensitivity(sensitivity);
                console.log('AudioManager initialized with sensitivity:', sensitivity);
            }

            // Connect audio source
            let result;
            if (this.currentSource === 'file' && this.fileInput.files.length > 0) {
                console.log('Connecting to file source...');
                result = await this.audioManager.connectFile(this.fileInput.files[0]);
            } else {
                console.log('Connecting to audio source:', this.currentSource);
                result = await this.connectAudioSource();
            }

            if (!result.success) {
                throw new Error(result.message);
            }

            console.log('Audio source connected:', result.message);

            // Resume audio context
            await this.audioManager.resume();

            // Initialize visualizer
            if (!this.visualizer) {
                console.log('Creating VisualizerEngine...');
                this.visualizer = new VisualizerEngine(this.canvasContainer);
                this.visualizer.updateText(this.textInput.value);

                // Set initial mode
                const activeMode = document.querySelector('.mode-btn.active');
                if (activeMode) {
                    this.visualizer.setMode(activeMode.dataset.mode);
                    console.log('Visualizer mode set to:', activeMode.dataset.mode);
                }
            }

            // Start animation loop
            this.isRunning = true;
            this.animate();

            // Update UI
            this.btnText.textContent = 'Stop Visualizer';
            this.playIcon.style.display = 'none';
            this.stopIcon.style.display = 'block';
            this.startStopBtn.classList.add('active');
            this.updateStatus(result.message, 'active');

            console.log('Visualizer started successfully!');

        } catch (error) {
            console.error('Start error:', error);
            this.updateStatus(error.message || 'Failed to start', 'error');
            this.stop();
        }
    }

    /**
     * Stop the visualizer
     */
    async stop() {
        console.log('Stopping visualizer...');
        this.isRunning = false;

        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
            this.animationId = null;
        }

        if (this.audioManager) {
            await this.audioManager.stop();
        }

        // Update UI
        this.btnText.textContent = 'Start Visualizer';
        this.playIcon.style.display = 'block';
        this.stopIcon.style.display = 'none';
        this.startStopBtn.classList.remove('active');
        this.updateStatus('Stopped', '');
    }

    /**
     * Connect to the selected audio source
     */
    async connectAudioSource(file = null) {
        if (file) {
            return await this.audioManager.connectFile(file);
        }

        switch (this.currentSource) {
            case 'microphone':
                return await this.audioManager.connectMicrophone();
            case 'tab':
                return await this.audioManager.connectTabAudio();
            case 'file':
                if (this.fileInput.files.length > 0) {
                    return await this.audioManager.connectFile(this.fileInput.files[0]);
                } else {
                    return { success: false, message: 'Please select an audio file' };
                }
            default:
                return { success: false, message: 'Invalid audio source' };
        }
    }

    /**
     * Animation loop
     */
    animate() {
        if (!this.isRunning) return;

        // Update visualizer
        if (this.visualizer && this.audioManager) {
            this.visualizer.update(this.audioManager);
            this.visualizer.render();
        }

        // Continue animation
        this.animationId = requestAnimationFrame(() => this.animate());
    }

    /**
     * Update status message
     */
    updateStatus(message, className = '') {
        const statusText = this.statusElement.querySelector('.status-text');
        if (statusText) {
            statusText.textContent = message;
        }
        this.statusElement.className = 'status-bar ' + className;
    }

    /**
     * Cleanup
     */
    dispose() {
        this.stop();

        if (this.audioManager) {
            this.audioManager.dispose();
        }

        if (this.visualizer) {
            this.visualizer.dispose();
        }
    }
}

// Initialize app when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initApp);
} else {
    initApp();
}

function initApp() {
    console.log('DOM ready, initializing app...');
    const app = new MusicVisualizerApp();

    // Cleanup on page unload
    window.addEventListener('beforeunload', () => {
        app.dispose();
    });

    // Make app globally accessible for debugging
    window.musicVisualizerApp = app;

    console.log('%c🎵 Music Visualizer Ready!', 'color: #7b2ff7; font-size: 16px; font-weight: bold;');
    console.log('%cSelect an audio source and click "Start Visualizer" to begin.', 'color: #00d4ff;');
}
