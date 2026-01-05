//
//  NewProjectView.swift
//  SoccerPlayPlanner
//
//  Create a new project with formations
//

import SwiftUI

struct NewProjectView: View {
    @EnvironmentObject var projectManager: ProjectManager
    @Environment(\.dismiss) private var dismiss

    @State private var projectName: String = ""
    @State private var homeFormation: Formation = .formation433
    @State private var awayFormation: Formation = .formation442

    var body: some View {
        NavigationView {
            Form {
                Section("Project Details") {
                    TextField("Project Name", text: $projectName)
                }

                Section("Home Team Formation") {
                    Picker("Formation", selection: $homeFormation) {
                        ForEach(Formation.presets) { formation in
                            Text(formation.name).tag(formation)
                        }
                    }
                }

                Section("Away Team Formation") {
                    Picker("Formation", selection: $awayFormation) {
                        ForEach(Formation.presets) { formation in
                            Text(formation.name).tag(formation)
                        }
                    }
                }
            }
            .navigationTitle("New Project")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarLeading) {
                    Button("Cancel") {
                        dismiss()
                    }
                }

                ToolbarItem(placement: .navigationBarTrailing) {
                    Button("Create") {
                        createProject()
                    }
                    .disabled(projectName.isEmpty)
                }
            }
        }
    }

    private func createProject() {
        let project = Project.create(
            name: projectName,
            homeFormation: homeFormation,
            awayFormation: awayFormation
        )
        projectManager.addProject(project)
        dismiss()
    }
}
