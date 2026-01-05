//
//  MainEditorView.swift
//  SoccerPlayPlanner
//
//  Main editor interface with field, players, and timeline
//

import SwiftUI

struct MainEditorView: View {
    let project: Project
    @StateObject private var viewModel: EditorViewModel
    @Environment(\.dismiss) private var dismiss
    @State private var showingFormationPicker = false
    @State private var showingExportOptions = false
    @State private var selectedTeamForFormation: Team = .home

    init(project: Project) {
        self.project = project
        _viewModel = StateObject(wrappedValue: EditorViewModel(project: project))
    }

    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Field and Players
                GeometryReader { geometry in
                    ZStack {
                        // Field
                        FieldView()

                        // Players
                        ForEach(viewModel.project.allPlayers) { player in
                            if let position = viewModel.currentPlayerPositions[player.id] {
                                PlayerView(
                                    player: player,
                                    size: 40,
                                    isSelected: viewModel.selectedPlayer?.id == player.id,
                                    onDragChanged: { location in
                                        // Convert screen coordinates to normalized field coordinates
                                        let normalizedX = location.x / geometry.size.width
                                        let normalizedY = location.y / geometry.size.height
                                        let clampedPosition = CGPoint(
                                            x: max(0, min(1, normalizedX)),
                                            y: max(0, min(1, normalizedY))
                                        )
                                        viewModel.updatePlayerPosition(playerId: player.id, position: clampedPosition)
                                    },
                                    onDragEnded: {
                                        // Optionally auto-create keyframe on drag end
                                    }
                                )
                                .position(
                                    x: position.x * geometry.size.width,
                                    y: position.y * geometry.size.height
                                )
                                .onTapGesture {
                                    viewModel.selectPlayer(player)
                                }
                            }
                        }
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                }
                .background(Color.black.opacity(0.1))

                Divider()

                // Timeline and Controls
                VStack(spacing: 12) {
                    // Timeline with keyframes
                    TimelineView(viewModel: viewModel)
                        .frame(height: 60)

                    // Playback controls
                    PlaybackControlsView(viewModel: viewModel)
                        .padding(.horizontal)

                    // Action buttons
                    HStack(spacing: 16) {
                        Button(action: { viewModel.addKeyframe() }) {
                            Label("Add Keyframe", systemImage: "plus.circle.fill")
                        }
                        .buttonStyle(.bordered)

                        Button(action: {
                            selectedTeamForFormation = .home
                            showingFormationPicker = true
                        }) {
                            Label("Home Formation", systemImage: "person.3.fill")
                        }
                        .buttonStyle(.bordered)

                        Button(action: {
                            selectedTeamForFormation = .away
                            showingFormationPicker = true
                        }) {
                            Label("Away Formation", systemImage: "person.3")
                        }
                        .buttonStyle(.bordered)

                        Spacer()

                        Button(action: { showingExportOptions = true }) {
                            Image(systemName: "square.and.arrow.up")
                        }
                        .buttonStyle(.borderedProminent)
                    }
                    .padding(.horizontal)
                    .padding(.bottom, 8)
                }
                .background(Color(.systemBackground))
            }
            .navigationTitle(project.name)
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Close") {
                        dismiss()
                    }
                }
            }
            .sheet(isPresented: $showingFormationPicker) {
                FormationPickerView(
                    team: selectedTeamForFormation,
                    onFormationSelected: { formation in
                        viewModel.applyFormation(formation, to: selectedTeamForFormation)
                        showingFormationPicker = false
                    }
                )
            }
            .sheet(isPresented: $showingExportOptions) {
                ExportOptionsView(project: viewModel.project)
            }
        }
    }
}
