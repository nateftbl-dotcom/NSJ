//
//  AnimationInterpolator.swift
//  SoccerPlayPlanner
//
//  Handles smooth interpolation between keyframes
//

import Foundation
import SwiftUI

class AnimationInterpolator {
    // Interpolate player positions at a given time
    static func interpolatePositions(
        keyframes: [Keyframe],
        atTime time: TimeInterval
    ) -> [UUID: CGPoint] {
        // Sort keyframes by timestamp
        let sortedKeyframes = keyframes.sorted { $0.timestamp < $1.timestamp }

        guard !sortedKeyframes.isEmpty else { return [:] }

        // If before first keyframe, return first keyframe
        if time <= sortedKeyframes.first!.timestamp {
            return sortedKeyframes.first!.playerPositions
        }

        // If after last keyframe, return last keyframe
        if time >= sortedKeyframes.last!.timestamp {
            return sortedKeyframes.last!.playerPositions
        }

        // Find the two keyframes to interpolate between
        var prevKeyframe: Keyframe?
        var nextKeyframe: Keyframe?

        for i in 0..<sortedKeyframes.count - 1 {
            if sortedKeyframes[i].timestamp <= time && sortedKeyframes[i + 1].timestamp >= time {
                prevKeyframe = sortedKeyframes[i]
                nextKeyframe = sortedKeyframes[i + 1]
                break
            }
        }

        guard let prev = prevKeyframe, let next = nextKeyframe else {
            return sortedKeyframes.last!.playerPositions
        }

        // Calculate interpolation factor (0 to 1)
        let totalDuration = next.timestamp - prev.timestamp
        let elapsed = time - prev.timestamp
        let t = totalDuration > 0 ? elapsed / totalDuration : 0

        // Apply easing function for smoother movement
        let easedT = easeInOutCubic(t)

        // Interpolate positions for each player
        var interpolatedPositions: [UUID: CGPoint] = [:]

        for (playerId, startPos) in prev.playerPositions {
            if let endPos = next.playerPositions[playerId] {
                let x = lerp(start: startPos.x, end: endPos.x, t: easedT)
                let y = lerp(start: startPos.y, end: endPos.y, t: easedT)
                interpolatedPositions[playerId] = CGPoint(x: x, y: y)
            } else {
                interpolatedPositions[playerId] = startPos
            }
        }

        return interpolatedPositions
    }

    // Linear interpolation
    private static func lerp(start: CGFloat, end: CGFloat, t: Double) -> CGFloat {
        return start + (end - start) * CGFloat(t)
    }

    // Ease in-out cubic easing function for smoother animation
    private static func easeInOutCubic(_ t: Double) -> Double {
        if t < 0.5 {
            return 4 * t * t * t
        } else {
            let f = 2 * t - 2
            return 1 + f * f * f / 2
        }
    }

    // Get the active keyframe at a given time
    static func getActiveKeyframe(keyframes: [Keyframe], atTime time: TimeInterval) -> Keyframe? {
        let sortedKeyframes = keyframes.sorted { $0.timestamp < $1.timestamp }
        return sortedKeyframes.last { $0.timestamp <= time }
    }

    // Get the next keyframe after a given time
    static func getNextKeyframe(keyframes: [Keyframe], afterTime time: TimeInterval) -> Keyframe? {
        let sortedKeyframes = keyframes.sorted { $0.timestamp < $1.timestamp }
        return sortedKeyframes.first { $0.timestamp > time }
    }
}
