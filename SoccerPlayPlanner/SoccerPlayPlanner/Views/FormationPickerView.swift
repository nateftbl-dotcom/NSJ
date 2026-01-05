//
//  FormationPickerView.swift
//  SoccerPlayPlanner
//
//  Formation selection interface
//

import SwiftUI

struct FormationPickerView: View {
    let team: Team
    let onFormationSelected: (Formation) -> Void

    @Environment(\.dismiss) private var dismiss
    @State private var customFormations: [Formation] = []

    var body: some View {
        NavigationView {
            List {
                Section("Preset Formations") {
                    ForEach(Formation.presets) { formation in
                        Button(action: {
                            onFormationSelected(formation)
                        }) {
                            HStack {
                                Text(formation.name)
                                    .foregroundColor(.primary)
                                Spacer()
                                Image(systemName: "chevron.right")
                                    .foregroundColor(.secondary)
                            }
                        }
                    }
                }

                if !customFormations.isEmpty {
                    Section("Custom Formations") {
                        ForEach(customFormations) { formation in
                            Button(action: {
                                onFormationSelected(formation)
                            }) {
                                HStack {
                                    Text(formation.name)
                                        .foregroundColor(.primary)
                                    Spacer()
                                    Image(systemName: "star.fill")
                                        .foregroundColor(.yellow)
                                        .font(.caption)
                                }
                            }
                        }
                        .onDelete(perform: deleteCustomFormation)
                    }
                }
            }
            .navigationTitle("\(team == .home ? "Home" : "Away") Formation")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Cancel") {
                        dismiss()
                    }
                }
            }
        }
        .onAppear {
            loadCustomFormations()
        }
    }

    private func loadCustomFormations() {
        // Load from ProjectManager
        // For now, just empty array
        customFormations = []
    }

    private func deleteCustomFormation(at offsets: IndexSet) {
        customFormations.remove(atOffsets: offsets)
        // Save to ProjectManager
    }
}
