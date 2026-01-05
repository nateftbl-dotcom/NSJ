//
//  Player.swift
//  SoccerPlayPlanner
//
//  Represents a player on the field
//

import Foundation
import SwiftUI

enum Team: String, Codable {
    case home
    case away
}

struct Player: Identifiable, Codable, Equatable {
    let id: UUID
    var number: Int
    var team: Team
    var position: CGPoint // Position on the field (0-1 normalized coordinates)

    init(id: UUID = UUID(), number: Int, team: Team, position: CGPoint) {
        self.id = id
        self.number = number
        self.team = team
        self.position = position
    }

    var color: Color {
        switch team {
        case .home:
            return .blue
        case .away:
            return .red
        }
    }
}
