/**
 * Visualizers - Three.js based music visualizations
 */
export class VisualizerEngine {
    constructor(container) {
        this.container = container;
        this.scene = null;
        this.camera = null;
        this.renderer = null;
        this.textMesh = null;
        this.currentMode = 'spectrum';
        this.displayText = 'NateFTBL';

        // Visualizer objects
        this.spectrumBars = [];
        this.particles = [];
        this.particleSystem = null;

        this.init();
    }

    /**
     * Initialize Three.js scene
     */
    init() {
        // Scene
        this.scene = new THREE.Scene();
        this.scene.fog = new THREE.Fog(0x0a0a0a, 10, 50);

        // Camera
        this.camera = new THREE.PerspectiveCamera(
            75,
            this.container.clientWidth / this.container.clientHeight,
            0.1,
            1000
        );
        this.camera.position.z = 15;

        // Renderer
        this.renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
        this.renderer.setSize(this.container.clientWidth, this.container.clientHeight);
        this.renderer.setClearColor(0x0a0a0a, 1);
        this.container.appendChild(this.renderer.domElement);

        // Lights
        const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
        this.scene.add(ambientLight);

        const pointLight1 = new THREE.PointLight(0x00d4ff, 1, 100);
        pointLight1.position.set(10, 10, 10);
        this.scene.add(pointLight1);

        const pointLight2 = new THREE.PointLight(0x7b2ff7, 1, 100);
        pointLight2.position.set(-10, -10, 10);
        this.scene.add(pointLight2);

        // Create initial text
        this.createText(this.displayText);

        // Create visualizer elements
        this.createSpectrumBars();
        this.createParticleSystem();

        // Handle window resize
        window.addEventListener('resize', () => this.onWindowResize());
    }

    /**
     * Create 3D text mesh
     */
    createText(text) {
        // Remove existing text
        if (this.textMesh) {
            this.scene.remove(this.textMesh);
        }

        // Create text using TextGeometry alternative (shapes)
        const loader = new THREE.FontLoader();

        // We'll create simple text using planes and canvas texture
        const canvas = document.createElement('canvas');
        const context = canvas.getContext('2d');
        canvas.width = 1024;
        canvas.height = 256;

        // Draw text on canvas
        context.fillStyle = '#ffffff';
        context.font = 'bold 120px Arial';
        context.textAlign = 'center';
        context.textBaseline = 'middle';
        context.fillText(text, canvas.width / 2, canvas.height / 2);

        // Create texture from canvas
        const texture = new THREE.CanvasTexture(canvas);
        texture.needsUpdate = true;

        // Create plane geometry
        const geometry = new THREE.PlaneGeometry(10, 2.5);
        const material = new THREE.MeshBasicMaterial({
            map: texture,
            transparent: true,
            side: THREE.DoubleSide
        });

        this.textMesh = new THREE.Mesh(geometry, material);
        this.textMesh.position.set(0, 0, 0);
        this.scene.add(this.textMesh);

        this.displayText = text;
    }

    /**
     * Create spectrum bars in a circle
     */
    createSpectrumBars() {
        const barCount = 64;
        const radius = 8;

        // Remove existing bars
        this.spectrumBars.forEach(bar => this.scene.remove(bar));
        this.spectrumBars = [];

        for (let i = 0; i < barCount; i++) {
            const geometry = new THREE.BoxGeometry(0.2, 1, 0.2);
            const material = new THREE.MeshPhongMaterial({
                color: 0x00d4ff,
                emissive: 0x00d4ff,
                emissiveIntensity: 0.5,
                shininess: 100
            });

            const bar = new THREE.Mesh(geometry, material);

            // Position bars in a circle
            const angle = (i / barCount) * Math.PI * 2;
            bar.position.x = Math.cos(angle) * radius;
            bar.position.z = Math.sin(angle) * radius;

            // Rotate to face center
            bar.rotation.y = -angle;

            bar.userData = { angle, baseY: 0 };
            this.spectrumBars.push(bar);
            this.scene.add(bar);
        }
    }

    /**
     * Create particle system
     */
    createParticleSystem() {
        const particleCount = 500;
        const geometry = new THREE.BufferGeometry();
        const positions = new Float32Array(particleCount * 3);
        const colors = new Float32Array(particleCount * 3);
        const sizes = new Float32Array(particleCount);

        for (let i = 0; i < particleCount; i++) {
            const i3 = i * 3;

            // Random position in sphere around text
            const radius = 5 + Math.random() * 5;
            const theta = Math.random() * Math.PI * 2;
            const phi = Math.random() * Math.PI;

            positions[i3] = radius * Math.sin(phi) * Math.cos(theta);
            positions[i3 + 1] = radius * Math.sin(phi) * Math.sin(theta);
            positions[i3 + 2] = radius * Math.cos(phi);

            // Random colors
            colors[i3] = Math.random();
            colors[i3 + 1] = Math.random();
            colors[i3 + 2] = Math.random();

            // Random sizes
            sizes[i] = Math.random() * 2 + 1;

            // Store initial position for animation
            this.particles.push({
                baseRadius: radius,
                theta: theta,
                phi: phi,
                speed: 0.001 + Math.random() * 0.003
            });
        }

        geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));
        geometry.setAttribute('color', new THREE.BufferAttribute(colors, 3));
        geometry.setAttribute('size', new THREE.BufferAttribute(sizes, 1));

        const material = new THREE.PointsMaterial({
            size: 0.1,
            vertexColors: true,
            transparent: true,
            opacity: 0.8,
            blending: THREE.AdditiveBlending
        });

        this.particleSystem = new THREE.Points(geometry, material);
        this.scene.add(this.particleSystem);
    }

    /**
     * Update spectrum bars visualizer
     */
    updateSpectrumBars(frequencyData) {
        if (!frequencyData || this.spectrumBars.length === 0) return;

        const step = Math.floor(frequencyData.length / this.spectrumBars.length);

        this.spectrumBars.forEach((bar, i) => {
            const value = frequencyData[i * step] / 255;
            const scale = 0.5 + value * 5;

            bar.scale.y = scale;
            bar.position.y = scale / 2;

            // Color based on frequency value
            const hue = value * 0.7 + 0.5; // Blue to pink
            bar.material.color.setHSL(hue, 1, 0.5);
            bar.material.emissive.setHSL(hue, 1, 0.3);
        });

        // Rotate bars slowly
        this.spectrumBars.forEach(bar => {
            bar.rotation.y += 0.001;
        });
    }

    /**
     * Update particle system visualizer
     */
    updateParticleSystem(bass, mid, treble) {
        if (!this.particleSystem) return;

        const positions = this.particleSystem.geometry.attributes.position.array;
        const colors = this.particleSystem.geometry.attributes.color.array;
        const sizes = this.particleSystem.geometry.attributes.size.array;

        for (let i = 0; i < this.particles.length; i++) {
            const particle = this.particles[i];
            const i3 = i * 3;

            // Orbit animation
            particle.theta += particle.speed;

            // Pulse with bass
            const radiusScale = 1 + bass * 0.5;
            const radius = particle.baseRadius * radiusScale;

            // Update position
            positions[i3] = radius * Math.sin(particle.phi) * Math.cos(particle.theta);
            positions[i3 + 1] = radius * Math.sin(particle.phi) * Math.sin(particle.theta);
            positions[i3 + 2] = radius * Math.cos(particle.phi);

            // Update colors based on frequency
            colors[i3] = treble; // Red
            colors[i3 + 1] = mid; // Green
            colors[i3 + 2] = bass; // Blue

            // Update size with bass
            sizes[i] = (1 + bass * 2) * (Math.random() * 0.5 + 0.5);
        }

        this.particleSystem.geometry.attributes.position.needsUpdate = true;
        this.particleSystem.geometry.attributes.color.needsUpdate = true;
        this.particleSystem.geometry.attributes.size.needsUpdate = true;

        // Rotate entire particle system
        this.particleSystem.rotation.y += 0.002;
    }

    /**
     * Update color-shifting text visualizer
     */
    updateColorShiftText(bass, mid, treble) {
        if (!this.textMesh) return;

        // Shift colors based on frequency bands
        const canvas = document.createElement('canvas');
        const context = canvas.getContext('2d');
        canvas.width = 1024;
        canvas.height = 256;

        // Create gradient based on frequencies
        const gradient = context.createLinearGradient(0, 0, canvas.width, 0);
        gradient.addColorStop(0, `rgb(${bass * 255}, ${mid * 100}, ${treble * 255})`);
        gradient.addColorStop(0.5, `rgb(${treble * 255}, ${bass * 255}, ${mid * 200})`);
        gradient.addColorStop(1, `rgb(${mid * 255}, ${treble * 255}, ${bass * 100})`);

        context.fillStyle = gradient;
        context.font = 'bold 120px Arial';
        context.textAlign = 'center';
        context.textBaseline = 'middle';
        context.fillText(this.displayText, canvas.width / 2, canvas.height / 2);

        // Add glow effect
        context.shadowBlur = 20 + bass * 50;
        context.shadowColor = `rgba(${bass * 255}, ${mid * 255}, ${treble * 255}, 0.8)`;

        // Update texture
        const texture = new THREE.CanvasTexture(canvas);
        this.textMesh.material.map = texture;
        this.textMesh.material.needsUpdate = true;

        // Scale and rotate text with bass
        const scale = 1 + bass * 0.3;
        this.textMesh.scale.set(scale, scale, 1);
        this.textMesh.rotation.z = Math.sin(Date.now() * 0.001) * 0.1 * bass;
    }

    /**
     * Update visualization based on current mode
     */
    update(audioManager) {
        if (!audioManager || !audioManager.isActive) return;

        const frequencyData = audioManager.getFrequencyData();
        const bass = audioManager.getBass();
        const mid = audioManager.getMid();
        const treble = audioManager.getTreble();

        // Show/hide elements based on mode
        this.spectrumBars.forEach(bar => {
            bar.visible = this.currentMode === 'spectrum';
        });

        if (this.particleSystem) {
            this.particleSystem.visible = this.currentMode === 'particles';
        }

        // Update based on current mode
        switch (this.currentMode) {
            case 'spectrum':
                this.updateSpectrumBars(frequencyData);
                break;
            case 'particles':
                this.updateParticleSystem(bass, mid, treble);
                break;
            case 'colorshift':
                this.updateColorShiftText(bass, mid, treble);
                break;
        }

        // Gentle camera movement
        this.camera.position.x = Math.sin(Date.now() * 0.0005) * 2;
        this.camera.position.y = Math.cos(Date.now() * 0.0003) * 1;
        this.camera.lookAt(0, 0, 0);
    }

    /**
     * Set visualizer mode
     */
    setMode(mode) {
        this.currentMode = mode;
    }

    /**
     * Update display text
     */
    updateText(text) {
        this.createText(text || 'NateFTBL');
    }

    /**
     * Render the scene
     */
    render() {
        this.renderer.render(this.scene, this.camera);
    }

    /**
     * Handle window resize
     */
    onWindowResize() {
        this.camera.aspect = this.container.clientWidth / this.container.clientHeight;
        this.camera.updateProjectionMatrix();
        this.renderer.setSize(this.container.clientWidth, this.container.clientHeight);
    }

    /**
     * Dispose resources
     */
    dispose() {
        window.removeEventListener('resize', () => this.onWindowResize());

        this.spectrumBars.forEach(bar => {
            bar.geometry.dispose();
            bar.material.dispose();
            this.scene.remove(bar);
        });

        if (this.particleSystem) {
            this.particleSystem.geometry.dispose();
            this.particleSystem.material.dispose();
            this.scene.remove(this.particleSystem);
        }

        if (this.textMesh) {
            this.textMesh.geometry.dispose();
            this.textMesh.material.dispose();
            this.scene.remove(this.textMesh);
        }

        this.renderer.dispose();
        this.container.removeChild(this.renderer.domElement);
    }
}
