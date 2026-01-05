//
//  ProjectManager.swift
//  SoccerPlayPlanner
//
//  Manages project persistence and file operations
//

import Foundation
import Combine

class ProjectManager: ObservableObject {
    @Published var projects: [Project] = []

    private let fileManager = FileManager.default
    private var projectsDirectory: URL {
        let documentsDirectory = fileManager.urls(for: .documentDirectory, in: .userDomainMask)[0]
        return documentsDirectory.appendingPathComponent("SoccerPlayPlanner", isDirectory: true)
    }

    init() {
        createProjectsDirectoryIfNeeded()
        loadProjects()
    }

    // MARK: - Project Management

    func addProject(_ project: Project) {
        projects.append(project)
        saveProject(project)
    }

    func updateProject(_ project: Project) {
        if let index = projects.firstIndex(where: { $0.id == project.id }) {
            projects[index] = project
            saveProject(project)
        }
    }

    func deleteProject(_ project: Project) {
        projects.removeAll { $0.id == project.id }
        deleteProjectFile(project)
    }

    // MARK: - File Operations

    private func createProjectsDirectoryIfNeeded() {
        if !fileManager.fileExists(atPath: projectsDirectory.path) {
            try? fileManager.createDirectory(at: projectsDirectory, withIntermediateDirectories: true)
        }
    }

    private func projectFileURL(for project: Project) -> URL {
        projectsDirectory.appendingPathComponent("\(project.id.uuidString).json")
    }

    func saveProject(_ project: Project) {
        let encoder = JSONEncoder()
        encoder.dateEncodingStrategy = .iso8601

        do {
            let data = try encoder.encode(project)
            let fileURL = projectFileURL(for: project)
            try data.write(to: fileURL)
        } catch {
            print("Error saving project: \(error)")
        }
    }

    private func loadProjects() {
        do {
            let fileURLs = try fileManager.contentsOfDirectory(
                at: projectsDirectory,
                includingPropertiesForKeys: nil
            )

            let decoder = JSONDecoder()
            decoder.dateDecodingStrategy = .iso8601

            projects = fileURLs.compactMap { url in
                guard url.pathExtension == "json" else { return nil }
                guard let data = try? Data(contentsOf: url) else { return nil }
                return try? decoder.decode(Project.self, from: data)
            }

            // Sort by modified date
            projects.sort { $0.modifiedDate > $1.modifiedDate }
        } catch {
            print("Error loading projects: \(error)")
        }
    }

    private func deleteProjectFile(_ project: Project) {
        let fileURL = projectFileURL(for: project)
        try? fileManager.removeItem(at: fileURL)
    }

    // MARK: - Export

    func exportProject(_ project: Project) -> URL? {
        let encoder = JSONEncoder()
        encoder.dateEncodingStrategy = .iso8601
        encoder.outputFormatting = .prettyPrinted

        do {
            let data = try encoder.encode(project)
            let tempURL = fileManager.temporaryDirectory
                .appendingPathComponent("\(project.name).soccerplay")
            try data.write(to: tempURL)
            return tempURL
        } catch {
            print("Error exporting project: \(error)")
            return nil
        }
    }

    func importProject(from url: URL) -> Bool {
        let decoder = JSONDecoder()
        decoder.dateDecodingStrategy = .iso8601

        do {
            let data = try Data(contentsOf: url)
            let project = try decoder.decode(Project.self, from: data)
            addProject(project)
            return true
        } catch {
            print("Error importing project: \(error)")
            return false
        }
    }
}
