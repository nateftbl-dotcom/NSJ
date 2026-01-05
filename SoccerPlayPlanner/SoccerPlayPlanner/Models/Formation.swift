//
//  Formation.swift
//  SoccerPlayPlanner
//
//  Represents a team formation (preset or custom)
//

import Foundation
import SwiftUI

struct Formation: Identifiable, Codable {
    let id: UUID
    var name: String
    var playerPositions: [CGPoint] // Normalized positions for 11 players
    var isCustom: Bool

    init(id: UUID = UUID(), name: String, playerPositions: [CGPoint], isCustom: Bool = false) {
        self.id = id
        self.name = name
        self.playerPositions = playerPositions
        self.isCustom = isCustom
    }
}

// Extension for preset formations
extension Formation {
    static let presets: [Formation] = [
        .formation433,
        .formation442,
        .formation352,
        .formation4231,
        .formation532,
        .formation343,
        .formation451
    ]

    // 4-3-3 Formation
    static let formation433 = Formation(
        name: "4-3-3",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.15), // LB
            CGPoint(x: 0.25, y: 0.35), // CB
            CGPoint(x: 0.25, y: 0.65), // CB
            CGPoint(x: 0.25, y: 0.85), // RB
            CGPoint(x: 0.45, y: 0.3),  // CM
            CGPoint(x: 0.45, y: 0.5),  // CM
            CGPoint(x: 0.45, y: 0.7),  // CM
            CGPoint(x: 0.75, y: 0.2),  // LW
            CGPoint(x: 0.75, y: 0.5),  // ST
            CGPoint(x: 0.75, y: 0.8)   // RW
        ]
    )

    // 4-4-2 Formation
    static let formation442 = Formation(
        name: "4-4-2",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.15), // LB
            CGPoint(x: 0.25, y: 0.35), // CB
            CGPoint(x: 0.25, y: 0.65), // CB
            CGPoint(x: 0.25, y: 0.85), // RB
            CGPoint(x: 0.5, y: 0.2),   // LM
            CGPoint(x: 0.5, y: 0.4),   // CM
            CGPoint(x: 0.5, y: 0.6),   // CM
            CGPoint(x: 0.5, y: 0.8),   // RM
            CGPoint(x: 0.75, y: 0.4),  // ST
            CGPoint(x: 0.75, y: 0.6)   // ST
        ]
    )

    // 3-5-2 Formation
    static let formation352 = Formation(
        name: "3-5-2",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.25), // CB
            CGPoint(x: 0.25, y: 0.5),  // CB
            CGPoint(x: 0.25, y: 0.75), // CB
            CGPoint(x: 0.45, y: 0.15), // LWB
            CGPoint(x: 0.45, y: 0.35), // CM
            CGPoint(x: 0.45, y: 0.5),  // CM
            CGPoint(x: 0.45, y: 0.65), // CM
            CGPoint(x: 0.45, y: 0.85), // RWB
            CGPoint(x: 0.75, y: 0.4),  // ST
            CGPoint(x: 0.75, y: 0.6)   // ST
        ]
    )

    // 4-2-3-1 Formation
    static let formation4231 = Formation(
        name: "4-2-3-1",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.15), // LB
            CGPoint(x: 0.25, y: 0.35), // CB
            CGPoint(x: 0.25, y: 0.65), // CB
            CGPoint(x: 0.25, y: 0.85), // RB
            CGPoint(x: 0.4, y: 0.35),  // CDM
            CGPoint(x: 0.4, y: 0.65),  // CDM
            CGPoint(x: 0.6, y: 0.25),  // LM
            CGPoint(x: 0.6, y: 0.5),   // CAM
            CGPoint(x: 0.6, y: 0.75),  // RM
            CGPoint(x: 0.8, y: 0.5)    // ST
        ]
    )

    // 5-3-2 Formation
    static let formation532 = Formation(
        name: "5-3-2",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.1),  // LWB
            CGPoint(x: 0.25, y: 0.3),  // CB
            CGPoint(x: 0.25, y: 0.5),  // CB
            CGPoint(x: 0.25, y: 0.7),  // CB
            CGPoint(x: 0.25, y: 0.9),  // RWB
            CGPoint(x: 0.5, y: 0.3),   // CM
            CGPoint(x: 0.5, y: 0.5),   // CM
            CGPoint(x: 0.5, y: 0.7),   // CM
            CGPoint(x: 0.75, y: 0.4),  // ST
            CGPoint(x: 0.75, y: 0.6)   // ST
        ]
    )

    // 3-4-3 Formation
    static let formation343 = Formation(
        name: "3-4-3",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.25), // CB
            CGPoint(x: 0.25, y: 0.5),  // CB
            CGPoint(x: 0.25, y: 0.75), // CB
            CGPoint(x: 0.5, y: 0.2),   // LM
            CGPoint(x: 0.5, y: 0.4),   // CM
            CGPoint(x: 0.5, y: 0.6),   // CM
            CGPoint(x: 0.5, y: 0.8),   // RM
            CGPoint(x: 0.75, y: 0.25), // LW
            CGPoint(x: 0.75, y: 0.5),  // ST
            CGPoint(x: 0.75, y: 0.75)  // RW
        ]
    )

    // 4-5-1 Formation
    static let formation451 = Formation(
        name: "4-5-1",
        playerPositions: [
            CGPoint(x: 0.1, y: 0.5),   // GK
            CGPoint(x: 0.25, y: 0.15), // LB
            CGPoint(x: 0.25, y: 0.35), // CB
            CGPoint(x: 0.25, y: 0.65), // CB
            CGPoint(x: 0.25, y: 0.85), // RB
            CGPoint(x: 0.5, y: 0.2),   // LM
            CGPoint(x: 0.5, y: 0.35),  // CM
            CGPoint(x: 0.5, y: 0.5),   // CM
            CGPoint(x: 0.5, y: 0.65),  // CM
            CGPoint(x: 0.5, y: 0.8),   // RM
            CGPoint(x: 0.8, y: 0.5)    // ST
        ]
    )
}
