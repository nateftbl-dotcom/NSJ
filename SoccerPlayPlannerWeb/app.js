// Soccer Play Planner - Web Demo
// Full-featured tactical planning app

class SoccerPlayPlanner {
    constructor() {
        this.canvas = document.getElementById('field');
        this.ctx = this.canvas.getContext('2d');

        // State
        this.players = [];
        this.keyframes = [];
        this.currentTime = 0;
        this.duration = 10; // seconds
        this.isPlaying = false;
        this.playbackSpeed = 1;
        this.isLooping = false;
        this.selectedPlayer = null;
        this.draggedPlayer = null;
        this.selectedTeam = 'home';

        // Animation
        this.animationFrameId = null;
        this.lastTimestamp = 0;

        this.init();
    }

    init() {
        this.setupCanvas();
        this.setupPlayers();
        this.setupEventListeners();
        this.addInitialKeyframe();
        this.render();

        // Start render loop
        this.animate();
    }

    setupCanvas() {
        // Set canvas size (soccer field aspect ratio 1.5:1)
        const container = document.getElementById('field-container');
        const containerWidth = container.clientWidth - 32;
        const containerHeight = container.clientHeight - 32;

        const aspectRatio = 1.5;
        let width = containerWidth;
        let height = width / aspectRatio;

        if (height > containerHeight) {
            height = containerHeight;
            width = height * aspectRatio;
        }

        this.canvas.width = width;
        this.canvas.height = height;

        // Handle resize
        window.addEventListener('resize', () => {
            this.setupCanvas();
            this.render();
        });
    }

    setupPlayers() {
        // Create home team (4-3-3 formation)
        const homeFormation = [
            { x: 0.1, y: 0.5 },   // GK
            { x: 0.25, y: 0.15 }, // LB
            { x: 0.25, y: 0.35 }, // CB
            { x: 0.25, y: 0.65 }, // CB
            { x: 0.25, y: 0.85 }, // RB
            { x: 0.45, y: 0.3 },  // CM
            { x: 0.45, y: 0.5 },  // CM
            { x: 0.45, y: 0.7 },  // CM
            { x: 0.75, y: 0.2 },  // LW
            { x: 0.75, y: 0.5 },  // ST
            { x: 0.75, y: 0.8 }   // RW
        ];

        // Create away team (4-4-2 formation, mirrored)
        const awayFormation = [
            { x: 0.9, y: 0.5 },   // GK
            { x: 0.75, y: 0.85 }, // RB
            { x: 0.75, y: 0.65 }, // CB
            { x: 0.75, y: 0.35 }, // CB
            { x: 0.75, y: 0.15 }, // LB
            { x: 0.5, y: 0.8 },   // RM
            { x: 0.5, y: 0.6 },   // CM
            { x: 0.5, y: 0.4 },   // CM
            { x: 0.5, y: 0.2 },   // LM
            { x: 0.25, y: 0.6 },  // ST
            { x: 0.25, y: 0.4 }   // ST
        ];

        // Create player objects
        homeFormation.forEach((pos, i) => {
            this.players.push({
                id: `home-${i}`,
                number: i + 1,
                team: 'home',
                x: pos.x,
                y: pos.y
            });
        });

        awayFormation.forEach((pos, i) => {
            this.players.push({
                id: `away-${i}`,
                number: i + 1,
                team: 'away',
                x: pos.x,
                y: pos.y
            });
        });
    }

    setupEventListeners() {
        // Canvas events (mouse and touch)
        this.canvas.addEventListener('mousedown', (e) => this.handlePointerDown(e));
        this.canvas.addEventListener('mousemove', (e) => this.handlePointerMove(e));
        this.canvas.addEventListener('mouseup', (e) => this.handlePointerUp(e));

        this.canvas.addEventListener('touchstart', (e) => this.handlePointerDown(e));
        this.canvas.addEventListener('touchmove', (e) => this.handlePointerMove(e));
        this.canvas.addEventListener('touchend', (e) => this.handlePointerUp(e));

        // Timeline events
        const timeline = document.getElementById('timeline');
        const handle = document.getElementById('timeline-handle');

        let isDraggingTimeline = false;

        const updateTimeFromEvent = (e) => {
            const rect = timeline.getBoundingClientRect();
            const x = (e.clientX || e.touches[0].clientX) - rect.left;
            const percent = Math.max(0, Math.min(1, x / rect.width));
            this.currentTime = percent * this.duration;
            this.updatePlayerPositions();
            this.render();
            this.updateTimeDisplay();
        };

        timeline.addEventListener('mousedown', (e) => {
            isDraggingTimeline = true;
            updateTimeFromEvent(e);
        });

        timeline.addEventListener('touchstart', (e) => {
            e.preventDefault();
            isDraggingTimeline = true;
            updateTimeFromEvent(e);
        });

        document.addEventListener('mousemove', (e) => {
            if (isDraggingTimeline) {
                updateTimeFromEvent(e);
            }
        });

        document.addEventListener('touchmove', (e) => {
            if (isDraggingTimeline) {
                e.preventDefault();
                updateTimeFromEvent(e);
            }
        });

        document.addEventListener('mouseup', () => {
            isDraggingTimeline = false;
        });

        document.addEventListener('touchend', () => {
            isDraggingTimeline = false;
        });

        // Button events
        document.getElementById('btn-play').addEventListener('click', () => this.togglePlay());
        document.getElementById('btn-stop').addEventListener('click', () => this.stop());
        document.getElementById('btn-loop').addEventListener('click', () => this.toggleLoop());
        document.getElementById('btn-add-keyframe').addEventListener('click', () => this.addKeyframe());
        document.getElementById('btn-home-formation').addEventListener('click', () => this.showFormationModal('home'));
        document.getElementById('btn-away-formation').addEventListener('click', () => this.showFormationModal('away'));
        document.getElementById('speed-select').addEventListener('change', (e) => {
            this.playbackSpeed = parseFloat(e.target.value);
        });
    }

    handlePointerDown(e) {
        e.preventDefault();
        const pos = this.getPointerPosition(e);
        const player = this.getPlayerAtPosition(pos.x, pos.y);

        if (player) {
            this.selectedPlayer = player;
            this.draggedPlayer = player;
            this.render();
        }
    }

    handlePointerMove(e) {
        e.preventDefault();
        if (this.draggedPlayer) {
            const pos = this.getPointerPosition(e);
            // Normalize to 0-1
            this.draggedPlayer.x = Math.max(0, Math.min(1, pos.x / this.canvas.width));
            this.draggedPlayer.y = Math.max(0, Math.min(1, pos.y / this.canvas.height));
            this.render();
        }
    }

    handlePointerUp(e) {
        e.preventDefault();
        this.draggedPlayer = null;
    }

    getPointerPosition(e) {
        const rect = this.canvas.getBoundingClientRect();
        const clientX = e.clientX || (e.touches && e.touches[0].clientX);
        const clientY = e.clientY || (e.touches && e.touches[0].clientY);

        return {
            x: clientX - rect.left,
            y: clientY - rect.top
        };
    }

    getPlayerAtPosition(x, y) {
        const playerRadius = 18; // Half of player size

        for (let i = this.players.length - 1; i >= 0; i--) {
            const player = this.players[i];
            const px = player.x * this.canvas.width;
            const py = player.y * this.canvas.height;
            const dist = Math.sqrt((x - px) ** 2 + (y - py) ** 2);

            if (dist < playerRadius) {
                return player;
            }
        }

        return null;
    }

    // Keyframe management
    addKeyframe() {
        // Save current player positions
        const positions = {};
        this.players.forEach(player => {
            positions[player.id] = { x: player.x, y: player.y };
        });

        const keyframe = {
            id: Date.now(),
            timestamp: this.currentTime,
            positions: positions
        };

        this.keyframes.push(keyframe);
        this.keyframes.sort((a, b) => a.timestamp - b.timestamp);

        this.render();
        this.renderTimeline();
    }

    addInitialKeyframe() {
        this.addKeyframe();
    }

    // Animation and playback
    togglePlay() {
        this.isPlaying = !this.isPlaying;
        const btn = document.getElementById('btn-play');

        if (this.isPlaying) {
            btn.innerHTML = '⏸ Pause';
            btn.classList.remove('primary');
            this.lastTimestamp = performance.now();
        } else {
            btn.innerHTML = '▶ Play';
            btn.classList.add('primary');
        }
    }

    stop() {
        this.isPlaying = false;
        this.currentTime = 0;
        document.getElementById('btn-play').innerHTML = '▶ Play';
        document.getElementById('btn-play').classList.add('primary');
        this.updatePlayerPositions();
        this.render();
        this.updateTimeDisplay();
    }

    toggleLoop() {
        this.isLooping = !this.isLooping;
        const btn = document.getElementById('btn-loop');
        btn.style.opacity = this.isLooping ? '1' : '0.6';
    }

    animate() {
        const now = performance.now();

        if (this.isPlaying) {
            const delta = (now - this.lastTimestamp) / 1000; // Convert to seconds
            this.currentTime += delta * this.playbackSpeed;

            if (this.currentTime >= this.duration) {
                if (this.isLooping) {
                    this.currentTime = 0;
                } else {
                    this.currentTime = this.duration;
                    this.isPlaying = false;
                    document.getElementById('btn-play').innerHTML = '▶ Play';
                    document.getElementById('btn-play').classList.add('primary');
                }
            }

            this.updatePlayerPositions();
            this.render();
            this.updateTimeDisplay();
        }

        this.lastTimestamp = now;
        requestAnimationFrame(() => this.animate());
    }

    updatePlayerPositions() {
        if (this.keyframes.length === 0) return;

        // Find surrounding keyframes
        let prevKeyframe = null;
        let nextKeyframe = null;

        for (let i = 0; i < this.keyframes.length; i++) {
            if (this.keyframes[i].timestamp <= this.currentTime) {
                prevKeyframe = this.keyframes[i];
            }
            if (this.keyframes[i].timestamp > this.currentTime && !nextKeyframe) {
                nextKeyframe = this.keyframes[i];
            }
        }

        if (!prevKeyframe) {
            prevKeyframe = this.keyframes[0];
        }

        if (!nextKeyframe) {
            // Use last keyframe
            this.players.forEach(player => {
                const pos = prevKeyframe.positions[player.id];
                if (pos) {
                    player.x = pos.x;
                    player.y = pos.y;
                }
            });
            return;
        }

        // Interpolate between keyframes
        const totalDuration = nextKeyframe.timestamp - prevKeyframe.timestamp;
        const elapsed = this.currentTime - prevKeyframe.timestamp;
        const t = totalDuration > 0 ? elapsed / totalDuration : 0;

        // Apply easing (ease-in-out cubic)
        const easedT = this.easeInOutCubic(t);

        this.players.forEach(player => {
            const startPos = prevKeyframe.positions[player.id];
            const endPos = nextKeyframe.positions[player.id];

            if (startPos && endPos) {
                player.x = this.lerp(startPos.x, endPos.x, easedT);
                player.y = this.lerp(startPos.y, endPos.y, easedT);
            }
        });
    }

    lerp(start, end, t) {
        return start + (end - start) * t;
    }

    easeInOutCubic(t) {
        if (t < 0.5) {
            return 4 * t * t * t;
        } else {
            const f = 2 * t - 2;
            return 1 + f * f * f / 2;
        }
    }

    updateTimeDisplay() {
        const current = this.formatTime(this.currentTime);
        const total = this.formatTime(this.duration);
        document.getElementById('time-display').textContent = `${current} / ${total}`;

        // Update timeline progress
        const percent = (this.currentTime / this.duration) * 100;
        document.getElementById('timeline-progress').style.width = `${percent}%`;
        document.getElementById('timeline-handle').style.left = `${percent}%`;
    }

    formatTime(seconds) {
        const mins = Math.floor(seconds / 60);
        const secs = Math.floor(seconds % 60);
        const ms = Math.floor((seconds % 1) * 100);
        return `${String(mins).padStart(2, '0')}:${String(secs).padStart(2, '0')}.${String(ms).padStart(2, '0')}`;
    }

    // Rendering
    render() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        this.drawField();
        this.drawPlayers();
        this.renderTimeline();
    }

    drawField() {
        const ctx = this.ctx;
        const w = this.canvas.width;
        const h = this.canvas.height;

        // Field background
        ctx.fillStyle = '#2d7a3e';
        ctx.fillRect(0, 0, w, h);

        // Stripes (optional, looks nice)
        ctx.fillStyle = 'rgba(0,0,0,0.05)';
        const stripeWidth = w / 10;
        for (let i = 0; i < 10; i += 2) {
            ctx.fillRect(i * stripeWidth, 0, stripeWidth, h);
        }

        // Field lines
        ctx.strokeStyle = '#fff';
        ctx.lineWidth = 2;

        // Outer boundary
        ctx.strokeRect(0, 0, w, h);

        // Halfway line
        ctx.beginPath();
        ctx.moveTo(w / 2, 0);
        ctx.lineTo(w / 2, h);
        ctx.stroke();

        // Center circle
        ctx.beginPath();
        ctx.arc(w / 2, h / 2, h * 0.15, 0, Math.PI * 2);
        ctx.stroke();

        // Center spot
        ctx.beginPath();
        ctx.arc(w / 2, h / 2, 3, 0, Math.PI * 2);
        ctx.fill();

        // Penalty areas
        const penaltyWidth = w * 0.15;
        const penaltyHeight = h * 0.6;

        // Left penalty area
        ctx.strokeRect(0, (h - penaltyHeight) / 2, penaltyWidth, penaltyHeight);

        // Right penalty area
        ctx.strokeRect(w - penaltyWidth, (h - penaltyHeight) / 2, penaltyWidth, penaltyHeight);

        // Goal areas
        const goalWidth = w * 0.05;
        const goalHeight = h * 0.35;

        ctx.strokeRect(0, (h - goalHeight) / 2, goalWidth, goalHeight);
        ctx.strokeRect(w - goalWidth, (h - goalHeight) / 2, goalWidth, goalHeight);

        // Penalty spots
        const penaltySpotDistance = w * 0.10;
        ctx.beginPath();
        ctx.arc(penaltySpotDistance, h / 2, 3, 0, Math.PI * 2);
        ctx.fill();

        ctx.beginPath();
        ctx.arc(w - penaltySpotDistance, h / 2, 3, 0, Math.PI * 2);
        ctx.fill();
    }

    drawPlayers() {
        this.players.forEach(player => {
            const x = player.x * this.canvas.width;
            const y = player.y * this.canvas.height;
            const radius = 18;

            // Shadow
            this.ctx.fillStyle = 'rgba(0,0,0,0.3)';
            this.ctx.beginPath();
            this.ctx.arc(x + 2, y + 2, radius, 0, Math.PI * 2);
            this.ctx.fill();

            // Player circle
            this.ctx.fillStyle = player.team === 'home' ? '#3b82f6' : '#ef4444';
            this.ctx.beginPath();
            this.ctx.arc(x, y, radius, 0, Math.PI * 2);
            this.ctx.fill();

            // Border
            this.ctx.strokeStyle = this.selectedPlayer === player ? '#fbbf24' : '#fff';
            this.ctx.lineWidth = this.selectedPlayer === player ? 3 : 2;
            this.ctx.stroke();

            // Number
            this.ctx.fillStyle = '#fff';
            this.ctx.font = 'bold 14px sans-serif';
            this.ctx.textAlign = 'center';
            this.ctx.textBaseline = 'middle';
            this.ctx.fillText(player.number, x, y);
        });
    }

    renderTimeline() {
        const timeline = document.getElementById('timeline');

        // Remove old markers
        const oldMarkers = timeline.querySelectorAll('.keyframe-marker');
        oldMarkers.forEach(marker => marker.remove());

        // Add keyframe markers
        this.keyframes.forEach(keyframe => {
            const percent = (keyframe.timestamp / this.duration) * 100;
            const marker = document.createElement('div');
            marker.className = 'keyframe-marker';
            marker.style.left = `${percent}%`;
            marker.addEventListener('click', () => {
                this.currentTime = keyframe.timestamp;
                this.updatePlayerPositions();
                this.render();
                this.updateTimeDisplay();
            });
            timeline.appendChild(marker);
        });
    }

    // Formation management
    showFormationModal(team) {
        this.selectedTeam = team;
        const modal = document.getElementById('formation-modal');
        const title = document.getElementById('modal-title');
        const list = document.getElementById('formation-list');

        title.textContent = `${team === 'home' ? 'Home' : 'Away'} Team Formation`;

        const formations = this.getFormations();
        list.innerHTML = '';

        formations.forEach(formation => {
            const item = document.createElement('div');
            item.className = 'formation-item';
            item.innerHTML = `<div class="formation-name">${formation.name}</div>`;
            item.addEventListener('click', () => {
                this.applyFormation(formation, team);
                this.closeFormationModal();
            });
            list.appendChild(item);
        });

        modal.classList.add('active');
    }

    closeFormationModal() {
        document.getElementById('formation-modal').classList.remove('active');
    }

    getFormations() {
        return [
            {
                name: '4-3-3',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.15 }, { x: 0.25, y: 0.35 },
                    { x: 0.25, y: 0.65 }, { x: 0.25, y: 0.85 }, { x: 0.45, y: 0.3 },
                    { x: 0.45, y: 0.5 }, { x: 0.45, y: 0.7 }, { x: 0.75, y: 0.2 },
                    { x: 0.75, y: 0.5 }, { x: 0.75, y: 0.8 }
                ]
            },
            {
                name: '4-4-2',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.15 }, { x: 0.25, y: 0.35 },
                    { x: 0.25, y: 0.65 }, { x: 0.25, y: 0.85 }, { x: 0.5, y: 0.2 },
                    { x: 0.5, y: 0.4 }, { x: 0.5, y: 0.6 }, { x: 0.5, y: 0.8 },
                    { x: 0.75, y: 0.4 }, { x: 0.75, y: 0.6 }
                ]
            },
            {
                name: '3-5-2',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.25 }, { x: 0.25, y: 0.5 },
                    { x: 0.25, y: 0.75 }, { x: 0.45, y: 0.15 }, { x: 0.45, y: 0.35 },
                    { x: 0.45, y: 0.5 }, { x: 0.45, y: 0.65 }, { x: 0.45, y: 0.85 },
                    { x: 0.75, y: 0.4 }, { x: 0.75, y: 0.6 }
                ]
            },
            {
                name: '4-2-3-1',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.15 }, { x: 0.25, y: 0.35 },
                    { x: 0.25, y: 0.65 }, { x: 0.25, y: 0.85 }, { x: 0.4, y: 0.35 },
                    { x: 0.4, y: 0.65 }, { x: 0.6, y: 0.25 }, { x: 0.6, y: 0.5 },
                    { x: 0.6, y: 0.75 }, { x: 0.8, y: 0.5 }
                ]
            },
            {
                name: '5-3-2',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.1 }, { x: 0.25, y: 0.3 },
                    { x: 0.25, y: 0.5 }, { x: 0.25, y: 0.7 }, { x: 0.25, y: 0.9 },
                    { x: 0.5, y: 0.3 }, { x: 0.5, y: 0.5 }, { x: 0.5, y: 0.7 },
                    { x: 0.75, y: 0.4 }, { x: 0.75, y: 0.6 }
                ]
            },
            {
                name: '3-4-3',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.25 }, { x: 0.25, y: 0.5 },
                    { x: 0.25, y: 0.75 }, { x: 0.5, y: 0.2 }, { x: 0.5, y: 0.4 },
                    { x: 0.5, y: 0.6 }, { x: 0.5, y: 0.8 }, { x: 0.75, y: 0.25 },
                    { x: 0.75, y: 0.5 }, { x: 0.75, y: 0.75 }
                ]
            },
            {
                name: '4-5-1',
                positions: [
                    { x: 0.1, y: 0.5 }, { x: 0.25, y: 0.15 }, { x: 0.25, y: 0.35 },
                    { x: 0.25, y: 0.65 }, { x: 0.25, y: 0.85 }, { x: 0.5, y: 0.2 },
                    { x: 0.5, y: 0.35 }, { x: 0.5, y: 0.5 }, { x: 0.5, y: 0.65 },
                    { x: 0.5, y: 0.8 }, { x: 0.8, y: 0.5 }
                ]
            }
        ];
    }

    applyFormation(formation, team) {
        const teamPlayers = this.players.filter(p => p.team === team);

        formation.positions.forEach((pos, i) => {
            if (i < teamPlayers.length) {
                const isMirrored = team === 'away';
                teamPlayers[i].x = isMirrored ? 1 - pos.x : pos.x;
                teamPlayers[i].y = pos.y;
            }
        });

        this.render();
    }
}

// Make closeFormationModal globally accessible
function closeFormationModal() {
    document.getElementById('formation-modal').classList.remove('active');
}

// Initialize the app
document.addEventListener('DOMContentLoaded', () => {
    window.app = new SoccerPlayPlanner();
});
