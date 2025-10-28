/**
 * AudioManager - Handles audio input from various sources and provides FFT analysis
 */
export class AudioManager {
    constructor() {
        this.audioContext = null;
        this.analyser = null;
        this.dataArray = null;
        this.bufferLength = 0;
        this.source = null;
        this.stream = null;
        this.isActive = false;
        this.sensitivity = 5.0;

        // FFT size for better frequency resolution
        this.fftSize = 2048;
    }

    /**
     * Initialize the audio context and analyser
     */
    async init() {
        try {
            this.audioContext = new (window.AudioContext || window.webkitAudioContext)();
            this.analyser = this.audioContext.createAnalyser();
            this.analyser.fftSize = this.fftSize;
            this.analyser.smoothingTimeConstant = 0.8;

            this.bufferLength = this.analyser.frequencyBinCount;
            this.dataArray = new Uint8Array(this.bufferLength);

            return true;
        } catch (error) {
            console.error('Failed to initialize audio context:', error);
            return false;
        }
    }

    /**
     * Connect to microphone input
     */
    async connectMicrophone() {
        try {
            await this.stop();

            this.stream = await navigator.mediaDevices.getUserMedia({
                audio: {
                    echoCancellation: false,
                    noiseSuppression: false,
                    autoGainControl: false
                }
            });

            this.source = this.audioContext.createMediaStreamSource(this.stream);
            this.source.connect(this.analyser);
            this.isActive = true;

            return { success: true, message: 'Microphone connected' };
        } catch (error) {
            console.error('Microphone access error:', error);
            return { success: false, message: 'Failed to access microphone' };
        }
    }

    /**
     * Connect to browser tab or system audio using Screen Capture API
     */
    async connectTabAudio() {
        try {
            await this.stop();

            // Request display media with audio
            this.stream = await navigator.mediaDevices.getDisplayMedia({
                video: true,
                audio: {
                    echoCancellation: false,
                    noiseSuppression: false,
                    autoGainControl: false
                }
            });

            // Check if audio track is available
            const audioTracks = this.stream.getAudioTracks();
            if (audioTracks.length === 0) {
                throw new Error('No audio track in selected source. Make sure to check "Share audio" when selecting.');
            }

            // Stop the video track as we only need audio
            const videoTracks = this.stream.getVideoTracks();
            videoTracks.forEach(track => track.stop());

            this.source = this.audioContext.createMediaStreamSource(this.stream);
            this.source.connect(this.analyser);
            this.isActive = true;

            return { success: true, message: 'Tab audio connected' };
        } catch (error) {
            console.error('Tab audio access error:', error);
            return {
                success: false,
                message: 'Failed to capture tab audio. Make sure to select "Share audio" option.'
            };
        }
    }

    /**
     * Connect to uploaded audio file
     */
    async connectFile(file) {
        try {
            await this.stop();

            const arrayBuffer = await file.arrayBuffer();
            const audioBuffer = await this.audioContext.decodeAudioData(arrayBuffer);

            this.source = this.audioContext.createBufferSource();
            this.source.buffer = audioBuffer;
            this.source.loop = true;
            this.source.connect(this.analyser);
            this.source.start(0);
            this.isActive = true;

            return { success: true, message: `Playing ${file.name}` };
        } catch (error) {
            console.error('File loading error:', error);
            return { success: false, message: 'Failed to load audio file' };
        }
    }

    /**
     * Stop current audio source
     */
    async stop() {
        if (this.source) {
            try {
                if (this.source.stop) {
                    this.source.stop();
                }
                this.source.disconnect();
            } catch (e) {
                // Source might already be stopped
            }
            this.source = null;
        }

        if (this.stream) {
            this.stream.getTracks().forEach(track => track.stop());
            this.stream = null;
        }

        this.isActive = false;
    }

    /**
     * Get frequency data (spectrum)
     */
    getFrequencyData() {
        if (!this.analyser || !this.isActive) {
            return new Uint8Array(this.bufferLength).fill(0);
        }

        this.analyser.getByteFrequencyData(this.dataArray);
        return this.dataArray;
    }

    /**
     * Get time domain data (waveform)
     */
    getTimeDomainData() {
        if (!this.analyser || !this.isActive) {
            return new Uint8Array(this.bufferLength).fill(128);
        }

        const timeData = new Uint8Array(this.bufferLength);
        this.analyser.getByteTimeDomainData(timeData);
        return timeData;
    }

    /**
     * Get average frequency in a specific range
     */
    getAverageFrequency(startIndex, endIndex) {
        const data = this.getFrequencyData();
        const slice = data.slice(startIndex, endIndex);
        const average = slice.reduce((a, b) => a + b, 0) / slice.length;
        return (average / 255) * this.sensitivity;
    }

    /**
     * Get bass level (low frequencies)
     */
    getBass() {
        return this.getAverageFrequency(0, Math.floor(this.bufferLength * 0.1));
    }

    /**
     * Get mid level (mid frequencies)
     */
    getMid() {
        const start = Math.floor(this.bufferLength * 0.1);
        const end = Math.floor(this.bufferLength * 0.4);
        return this.getAverageFrequency(start, end);
    }

    /**
     * Get treble level (high frequencies)
     */
    getTreble() {
        const start = Math.floor(this.bufferLength * 0.4);
        return this.getAverageFrequency(start, this.bufferLength);
    }

    /**
     * Set sensitivity multiplier
     */
    setSensitivity(value) {
        this.sensitivity = value;
    }

    /**
     * Resume audio context (needed after user interaction)
     */
    async resume() {
        if (this.audioContext && this.audioContext.state === 'suspended') {
            await this.audioContext.resume();
        }
    }

    /**
     * Clean up resources
     */
    dispose() {
        this.stop();
        if (this.audioContext) {
            this.audioContext.close();
            this.audioContext = null;
        }
    }
}
