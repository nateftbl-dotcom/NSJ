//
//  SoccerPlayPlannerApp.swift
//  SoccerPlayPlanner
//
//  Main app entry point
//

import SwiftUI

@main
struct SoccerPlayPlannerApp: App {
    @StateObject private var projectManager = ProjectManager()

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(projectManager)
        }
    }
}
