//
//  TimelineView.swift
//  SoccerPlayPlanner
//
//  Timeline visualization with keyframe markers
//

import SwiftUI

struct TimelineView: View {
    @ObservedObject var viewModel: EditorViewModel

    var body: some View {
        GeometryReader { geometry in
            ZStack(alignment: .leading) {
                // Timeline background
                Rectangle()
                    .fill(Color(.systemGray6))

                // Keyframe markers
                ForEach(viewModel.project.keyframes) { keyframe in
                    let xPosition = (keyframe.timestamp / viewModel.project.duration) * geometry.size.width

                    VStack(spacing: 2) {
                        Rectangle()
                            .fill(Color.blue)
                            .frame(width: 3)

                        Text(formatTime(keyframe.timestamp))
                            .font(.system(size: 8))
                            .foregroundColor(.secondary)
                    }
                    .frame(maxHeight: .infinity)
                    .offset(x: xPosition)
                    .gesture(
                        DragGesture()
                            .onChanged { value in
                                let newTime = (value.location.x / geometry.size.width) * viewModel.project.duration
                                let clampedTime = max(0, min(viewModel.project.duration, newTime))
                                var updatedKeyframe = keyframe
                                updatedKeyframe.timestamp = clampedTime
                                viewModel.updateKeyframe(updatedKeyframe)
                            }
                    )
                    .contextMenu {
                        Button(role: .destructive) {
                            viewModel.deleteKeyframe(keyframe)
                        } label: {
                            Label("Delete Keyframe", systemImage: "trash")
                        }
                    }
                }

                // Current time indicator
                Rectangle()
                    .fill(Color.red)
                    .frame(width: 2)
                    .offset(x: (viewModel.currentTime / viewModel.project.duration) * geometry.size.width)
            }
            .gesture(
                DragGesture(minimumDistance: 0)
                    .onChanged { value in
                        let newTime = (value.location.x / geometry.size.width) * viewModel.project.duration
                        viewModel.seek(to: newTime)
                    }
            )
        }
        .cornerRadius(4)
        .overlay(
            RoundedRectangle(cornerRadius: 4)
                .stroke(Color.gray.opacity(0.3), lineWidth: 1)
        )
        .padding(.horizontal)
    }

    private func formatTime(_ time: TimeInterval) -> String {
        let seconds = Int(time)
        return "\(seconds)s"
    }
}
