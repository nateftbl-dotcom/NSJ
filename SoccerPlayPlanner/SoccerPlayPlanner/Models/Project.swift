//
//  Project.swift
//  SoccerPlayPlanner
//
//  Represents a complete tactical project with multiple keyframes
//

import Foundation
import SwiftUI

struct Project: Identifiable, Codable {
    let id: UUID
    var name: String
    var createdDate: Date
    var modifiedDate: Date
    var homePlayers: [Player]
    var awayPlayers: [Player]
    var keyframes: [Keyframe]
    var duration: TimeInterval // Total animation duration in seconds

    init(
        id: UUID = UUID(),
        name: String,
        createdDate: Date = Date(),
        modifiedDate: Date = Date(),
        homePlayers: [Player] = [],
        awayPlayers: [Player] = [],
        keyframes: [Keyframe] = [],
        duration: TimeInterval = 10.0
    ) {
        self.id = id
        self.name = name
        self.createdDate = createdDate
        self.modifiedDate = modifiedDate
        self.homePlayers = homePlayers
        self.awayPlayers = awayPlayers
        self.keyframes = keyframes
        self.duration = duration
    }

    var allPlayers: [Player] {
        homePlayers + awayPlayers
    }

    mutating func updateModifiedDate() {
        modifiedDate = Date()
    }

    // Create a new project with formations
    static func create(
        name: String,
        homeFormation: Formation,
        awayFormation: Formation
    ) -> Project {
        // Create home team players (1-11)
        let homePlayers = homeFormation.playerPositions.enumerated().map { index, position in
            Player(number: index + 1, team: .home, position: position)
        }

        // Create away team players (1-11) - mirror positions horizontally
        let awayPlayers = awayFormation.playerPositions.enumerated().map { index, position in
            let mirroredPosition = CGPoint(x: 1.0 - position.x, y: position.y)
            return Player(number: index + 1, team: .away, position: mirroredPosition)
        }

        // Create initial keyframe at t=0
        let allPlayers = homePlayers + awayPlayers
        let initialKeyframe = Keyframe.from(players: allPlayers, timestamp: 0.0, notes: "Initial formation")

        return Project(
            name: name,
            homePlayers: homePlayers,
            awayPlayers: awayPlayers,
            keyframes: [initialKeyframe]
        )
    }
}
