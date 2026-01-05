//
//  ProjectListView.swift
//  SoccerPlayPlanner
//
//  List of saved projects
//

import SwiftUI

struct ProjectListView: View {
    @EnvironmentObject var projectManager: ProjectManager
    @Binding var selectedProject: Project?

    var body: some View {
        List {
            if projectManager.projects.isEmpty {
                VStack(spacing: 16) {
                    Image(systemName: "doc.text.magnifyingglass")
                        .font(.system(size: 60))
                        .foregroundColor(.secondary)

                    Text("No Projects Yet")
                        .font(.title2)
                        .foregroundColor(.secondary)

                    Text("Create your first soccer play by tapping the + button above")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                        .padding(.horizontal)
                }
                .frame(maxWidth: .infinity)
                .padding(.top, 100)
            } else {
                ForEach(projectManager.projects) { project in
                    ProjectRowView(project: project)
                        .contentShape(Rectangle())
                        .onTapGesture {
                            selectedProject = project
                        }
                }
                .onDelete(perform: deleteProjects)
            }
        }
    }

    private func deleteProjects(at offsets: IndexSet) {
        for index in offsets {
            let project = projectManager.projects[index]
            projectManager.deleteProject(project)
        }
    }
}

struct ProjectRowView: View {
    let project: Project

    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text(project.name)
                .font(.headline)

            HStack {
                Text("\(project.keyframes.count) keyframes")
                    .font(.caption)
                    .foregroundColor(.secondary)

                Spacer()

                Text(project.modifiedDate, style: .relative)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding(.vertical, 4)
    }
}
