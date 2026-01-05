//
//  Keyframe.swift
//  SoccerPlayPlanner
//
//  Represents a snapshot of all player positions at a specific time
//

import Foundation
import SwiftUI

struct Keyframe: Identifiable, Codable {
    let id: UUID
    var timestamp: TimeInterval // Time in seconds from start
    var playerPositions: [UUID: CGPoint] // Player ID to position mapping
    var notes: String // Optional notes for this keyframe

    init(id: UUID = UUID(), timestamp: TimeInterval, playerPositions: [UUID: CGPoint], notes: String = "") {
        self.id = id
        self.timestamp = timestamp
        self.playerPositions = playerPositions
        self.notes = notes
    }

    // Create a keyframe from current player positions
    static func from(players: [Player], timestamp: TimeInterval, notes: String = "") -> Keyframe {
        let positions = Dictionary(uniqueKeysWithValues: players.map { ($0.id, $0.position) })
        return Keyframe(timestamp: timestamp, playerPositions: positions, notes: notes)
    }
}
