//
//  ContentView.swift
//  SoccerPlayPlanner
//
//  Main navigation and project selection view
//

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var projectManager: ProjectManager
    @State private var showingNewProject = false
    @State private var selectedProject: Project?

    var body: some View {
        NavigationView {
            ProjectListView(selectedProject: $selectedProject)
                .navigationTitle("Soccer Play Planner")
                .toolbar {
                    ToolbarItem(placement: .navigationBarTrailing) {
                        Button(action: { showingNewProject = true }) {
                            Image(systemName: "plus")
                        }
                    }
                }
                .sheet(isPresented: $showingNewProject) {
                    NewProjectView()
                }
                .sheet(item: $selectedProject) { project in
                    MainEditorView(project: project)
                }
        }
        .navigationViewStyle(StackNavigationViewStyle())
    }
}
