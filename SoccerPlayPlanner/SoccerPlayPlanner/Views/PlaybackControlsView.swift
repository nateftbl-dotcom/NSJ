//
//  PlaybackControlsView.swift
//  SoccerPlayPlanner
//
//  Playback controls (play, pause, speed, loop)
//

import SwiftUI

struct PlaybackControlsView: View {
    @ObservedObject var viewModel: EditorViewModel

    var body: some View {
        VStack(spacing: 12) {
            // Time display and scrubber
            HStack {
                Text(formatTime(viewModel.currentTime))
                    .font(.system(.caption, design: .monospaced))
                    .frame(width: 50, alignment: .leading)

                Slider(
                    value: Binding(
                        get: { viewModel.currentTime },
                        set: { viewModel.seek(to: $0) }
                    ),
                    in: 0...viewModel.project.duration
                )

                Text(formatTime(viewModel.project.duration))
                    .font(.system(.caption, design: .monospaced))
                    .frame(width: 50, alignment: .trailing)
            }

            // Control buttons
            HStack(spacing: 24) {
                // Stop
                Button(action: { viewModel.stop() }) {
                    Image(systemName: "stop.fill")
                        .font(.title2)
                }

                // Play/Pause
                Button(action: {
                    if viewModel.isPlaying {
                        viewModel.pause()
                    } else {
                        viewModel.play()
                    }
                }) {
                    Image(systemName: viewModel.isPlaying ? "pause.fill" : "play.fill")
                        .font(.title)
                }

                // Loop toggle
                Button(action: { viewModel.isLooping.toggle() }) {
                    Image(systemName: viewModel.isLooping ? "repeat.1" : "repeat")
                        .font(.title2)
                        .foregroundColor(viewModel.isLooping ? .blue : .gray)
                }

                Spacer()

                // Speed control
                Menu {
                    Button("0.25x") { viewModel.playbackSpeed = 0.25 }
                    Button("0.5x") { viewModel.playbackSpeed = 0.5 }
                    Button("1x") { viewModel.playbackSpeed = 1.0 }
                    Button("1.5x") { viewModel.playbackSpeed = 1.5 }
                    Button("2x") { viewModel.playbackSpeed = 2.0 }
                } label: {
                    HStack {
                        Image(systemName: "gauge")
                        Text("\(String(format: "%.2f", viewModel.playbackSpeed))x")
                            .font(.caption)
                    }
                }
            }
        }
    }

    private func formatTime(_ time: TimeInterval) -> String {
        let minutes = Int(time) / 60
        let seconds = Int(time) % 60
        let milliseconds = Int((time.truncatingRemainder(dividingBy: 1)) * 100)
        return String(format: "%02d:%02d.%02d", minutes, seconds, milliseconds)
    }
}
