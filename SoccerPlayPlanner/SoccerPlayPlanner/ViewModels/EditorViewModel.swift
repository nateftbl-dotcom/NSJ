//
//  EditorViewModel.swift
//  SoccerPlayPlanner
//
//  Manages the state and logic for the main editor
//

import Foundation
import SwiftUI
import Combine

class EditorViewModel: ObservableObject {
    @Published var project: Project
    @Published var currentTime: TimeInterval = 0.0
    @Published var isPlaying: Bool = false
    @Published var playbackSpeed: Double = 1.0
    @Published var isLooping: Bool = false
    @Published var selectedPlayer: Player?
    @Published var currentPlayerPositions: [UUID: CGPoint] = [:]

    private var playbackTimer: Timer?
    private let timeStep: TimeInterval = 1.0 / 60.0 // 60 FPS

    init(project: Project) {
        self.project = project
        self.currentPlayerPositions = project.keyframes.first?.playerPositions ?? [:]
    }

    // MARK: - Playback Control

    func play() {
        guard !isPlaying else { return }
        isPlaying = true

        playbackTimer = Timer.scheduledTimer(withTimeInterval: timeStep, repeats: true) { [weak self] _ in
            guard let self = self else { return }

            self.currentTime += self.timeStep * self.playbackSpeed

            if self.currentTime >= self.project.duration {
                if self.isLooping {
                    self.currentTime = 0.0
                } else {
                    self.pause()
                    self.currentTime = self.project.duration
                }
            }

            self.updatePlayerPositions()
        }
    }

    func pause() {
        isPlaying = false
        playbackTimer?.invalidate()
        playbackTimer = nil
    }

    func stop() {
        pause()
        currentTime = 0.0
        updatePlayerPositions()
    }

    func seek(to time: TimeInterval) {
        currentTime = max(0, min(time, project.duration))
        updatePlayerPositions()
    }

    // MARK: - Keyframe Management

    func addKeyframe(at time: TimeInterval? = nil) {
        let timestamp = time ?? currentTime
        let newKeyframe = Keyframe.from(
            players: project.allPlayers,
            timestamp: timestamp,
            notes: ""
        )

        // Update current positions to match the new keyframe
        currentPlayerPositions = newKeyframe.playerPositions

        project.keyframes.append(newKeyframe)
        project.keyframes.sort { $0.timestamp < $1.timestamp }
        project.updateModifiedDate()
    }

    func deleteKeyframe(_ keyframe: Keyframe) {
        project.keyframes.removeAll { $0.id == keyframe.id }
        updatePlayerPositions()
        project.updateModifiedDate()
    }

    func updateKeyframe(_ keyframe: Keyframe) {
        if let index = project.keyframes.firstIndex(where: { $0.id == keyframe.id }) {
            project.keyframes[index] = keyframe
            project.updateModifiedDate()
        }
    }

    // MARK: - Player Management

    func updatePlayerPosition(playerId: UUID, position: CGPoint) {
        // Update in the current player positions
        currentPlayerPositions[playerId] = position

        // Update in the players array
        if let index = project.homePlayers.firstIndex(where: { $0.id == playerId }) {
            project.homePlayers[index].position = position
        } else if let index = project.awayPlayers.firstIndex(where: { $0.id == playerId }) {
            project.awayPlayers[index].position = position
        }

        // Update the nearest keyframe if it exists at current time
        if let keyframe = AnimationInterpolator.getActiveKeyframe(
            keyframes: project.keyframes,
            atTime: currentTime
        ), keyframe.timestamp == currentTime {
            var updatedKeyframe = keyframe
            updatedKeyframe.playerPositions[playerId] = position
            updateKeyframe(updatedKeyframe)
        }
    }

    func selectPlayer(_ player: Player?) {
        selectedPlayer = player
    }

    // MARK: - Formation Management

    func applyFormation(_ formation: Formation, to team: Team) {
        let players = team == .home ? project.homePlayers : project.awayPlayers
        let isMirrored = team == .away

        for (index, player) in players.enumerated() {
            guard index < formation.playerPositions.count else { break }

            var position = formation.playerPositions[index]
            if isMirrored {
                position = CGPoint(x: 1.0 - position.x, y: position.y)
            }

            updatePlayerPosition(playerId: player.id, position: position)
        }

        project.updateModifiedDate()
    }

    func saveCurrentFormation(name: String, for team: Team) -> Formation {
        let players = team == .home ? project.homePlayers : project.awayPlayers
        let positions = players.map { $0.position }

        return Formation(
            name: name,
            playerPositions: positions,
            isCustom: true
        )
    }

    // MARK: - Private Methods

    private func updatePlayerPositions() {
        currentPlayerPositions = AnimationInterpolator.interpolatePositions(
            keyframes: project.keyframes,
            atTime: currentTime
        )

        // Update player positions in the project
        for player in project.allPlayers {
            if let newPosition = currentPlayerPositions[player.id] {
                updatePlayerPosition(playerId: player.id, position: newPosition)
            }
        }
    }

    // MARK: - Cleanup

    deinit {
        playbackTimer?.invalidate()
    }
}
