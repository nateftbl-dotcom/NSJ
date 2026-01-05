//
//  ExportOptionsView.swift
//  SoccerPlayPlanner
//
//  Export and sharing options
//

import SwiftUI

struct ExportOptionsView: View {
    let project: Project
    @Environment(\.dismiss) private var dismiss
    @State private var showingShareSheet = false
    @State private var shareURL: URL?

    var body: some View {
        NavigationView {
            List {
                Section("Export Options") {
                    Button(action: exportAsVideo) {
                        Label("Export as Video", systemImage: "film")
                    }

                    Button(action: exportAsPDF) {
                        Label("Export as PDF", systemImage: "doc.richtext")
                    }

                    Button(action: exportAsProject) {
                        Label("Export Project File", systemImage: "doc.badge.arrow.up")
                    }
                }

                Section("Quick Actions") {
                    Button(action: shareProject) {
                        Label("Share Project", systemImage: "square.and.arrow.up")
                    }
                }
            }
            .navigationTitle("Export")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarTrailing) {
                    Button("Done") {
                        dismiss()
                    }
                }
            }
            .sheet(isPresented: $showingShareSheet) {
                if let url = shareURL {
                    ShareSheet(items: [url])
                }
            }
        }
    }

    private func exportAsVideo() {
        // TODO: Implement video export using AVFoundation
        // This would render each frame of the animation and compile into video
        print("Export as video - to be implemented")
    }

    private func exportAsPDF() {
        // TODO: Implement PDF export showing key formations
        print("Export as PDF - to be implemented")
    }

    private func exportAsProject() {
        let projectManager = ProjectManager()
        if let url = projectManager.exportProject(project) {
            shareURL = url
            showingShareSheet = true
        }
    }

    private func shareProject() {
        exportAsProject()
    }
}

// Share sheet for iOS
struct ShareSheet: UIViewControllerRepresentable {
    let items: [Any]

    func makeUIViewController(context: Context) -> UIActivityViewController {
        let controller = UIActivityViewController(activityItems: items, applicationActivities: nil)
        return controller
    }

    func updateUIViewController(_ uiViewController: UIActivityViewController, context: Context) {}
}
