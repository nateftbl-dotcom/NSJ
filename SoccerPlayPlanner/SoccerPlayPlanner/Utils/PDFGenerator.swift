//
//  PDFGenerator.swift
//  SoccerPlayPlanner
//
//  Generate PDF documents showing play diagrams
//

import Foundation
import PDFKit
import UIKit

class PDFGenerator {
    static func generatePDF(
        for project: Project,
        completion: @escaping (Result<URL, Error>) -> Void
    ) {
        let pdfMetaData = [
            kCGPDFContextCreator: "Soccer Play Planner",
            kCGPDFContextTitle: project.name,
            kCGPDFContextAuthor: "Soccer Play Planner"
        ]

        let format = UIGraphicsPDFRendererFormat()
        format.documentInfo = pdfMetaData as [String: Any]

        let pageRect = CGRect(x: 0, y: 0, width: 612, height: 792) // US Letter size
        let renderer = UIGraphicsPDFRenderer(bounds: pageRect, format: format)

        let outputURL = FileManager.default.temporaryDirectory
            .appendingPathComponent("\(project.name)_\(UUID().uuidString).pdf")

        do {
            try renderer.writePDF(to: outputURL) { context in
                // Title page
                context.beginPage()
                drawTitlePage(project: project, in: pageRect)

                // Create a page for each keyframe
                for keyframe in project.keyframes.sorted(by: { $0.timestamp < $1.timestamp }) {
                    context.beginPage()
                    drawKeyframePage(
                        project: project,
                        keyframe: keyframe,
                        in: pageRect
                    )
                }
            }

            completion(.success(outputURL))
        } catch {
            completion(.failure(error))
        }
    }

    private static func drawTitlePage(project: Project, in rect: CGRect) {
        // Title
        let titleAttributes: [NSAttributedString.Key: Any] = [
            .font: UIFont.boldSystemFont(ofSize: 36),
            .foregroundColor: UIColor.black
        ]

        let title = project.name as NSString
        let titleSize = title.size(withAttributes: titleAttributes)
        let titleRect = CGRect(
            x: (rect.width - titleSize.width) / 2,
            y: 100,
            width: titleSize.width,
            height: titleSize.height
        )
        title.draw(in: titleRect, withAttributes: titleAttributes)

        // Project info
        let infoAttributes: [NSAttributedString.Key: Any] = [
            .font: UIFont.systemFont(ofSize: 14),
            .foregroundColor: UIColor.darkGray
        ]

        let dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .long
        dateFormatter.timeStyle = .short

        let info = """
        Created: \(dateFormatter.string(from: project.createdDate))
        Duration: \(String(format: "%.1f", project.duration))s
        Keyframes: \(project.keyframes.count)
        """ as NSString

        let infoRect = CGRect(x: 50, y: 200, width: rect.width - 100, height: 200)
        info.draw(in: infoRect, withAttributes: infoAttributes)
    }

    private static func drawKeyframePage(
        project: Project,
        keyframe: Keyframe,
        in rect: CGRect
    ) {
        // Header
        let headerAttributes: [NSAttributedString.Key: Any] = [
            .font: UIFont.boldSystemFont(ofSize: 24),
            .foregroundColor: UIColor.black
        ]

        let header = "Keyframe at \(String(format: "%.1f", keyframe.timestamp))s" as NSString
        let headerRect = CGRect(x: 50, y: 50, width: rect.width - 100, height: 40)
        header.draw(in: headerRect, withAttributes: headerAttributes)

        // Field drawing area
        let fieldRect = CGRect(
            x: 50,
            y: 120,
            width: rect.width - 100,
            height: (rect.width - 100) / 1.5
        )

        // Draw field
        drawFieldDiagram(in: fieldRect)

        // Draw players at keyframe positions
        for player in project.allPlayers {
            if let position = keyframe.playerPositions[player.id] {
                let screenPosition = CGPoint(
                    x: fieldRect.minX + position.x * fieldRect.width,
                    y: fieldRect.minY + position.y * fieldRect.height
                )
                drawPlayerDiagram(player, at: screenPosition, size: 30)
            }
        }

        // Notes
        if !keyframe.notes.isEmpty {
            let notesAttributes: [NSAttributedString.Key: Any] = [
                .font: UIFont.systemFont(ofSize: 12),
                .foregroundColor: UIColor.darkGray
            ]

            let notesLabel = "Notes:" as NSString
            let notesLabelRect = CGRect(
                x: 50,
                y: fieldRect.maxY + 30,
                width: rect.width - 100,
                height: 20
            )
            notesLabel.draw(in: notesLabelRect, withAttributes: headerAttributes)

            let notes = keyframe.notes as NSString
            let notesRect = CGRect(
                x: 50,
                y: fieldRect.maxY + 55,
                width: rect.width - 100,
                height: 100
            )
            notes.draw(in: notesRect, withAttributes: notesAttributes)
        }
    }

    private static func drawFieldDiagram(in rect: CGRect) {
        guard let context = UIGraphicsGetCurrentContext() else { return }

        // Field background
        context.setFillColor(UIColor(red: 0.2, green: 0.6, blue: 0.2, alpha: 1.0).cgColor)
        context.fill(rect)

        // Field lines
        context.setStrokeColor(UIColor.white.cgColor)
        context.setLineWidth(2)

        // Boundary
        context.stroke(rect)

        // Halfway line
        context.move(to: CGPoint(x: rect.midX, y: rect.minY))
        context.addLine(to: CGPoint(x: rect.midX, y: rect.maxY))
        context.strokePath()

        // Center circle
        let centerCircleRadius = rect.height * 0.15
        context.addEllipse(in: CGRect(
            x: rect.midX - centerCircleRadius,
            y: rect.midY - centerCircleRadius,
            width: centerCircleRadius * 2,
            height: centerCircleRadius * 2
        ))
        context.strokePath()
    }

    private static func drawPlayerDiagram(_ player: Player, at position: CGPoint, size: CGFloat) {
        guard let context = UIGraphicsGetCurrentContext() else { return }

        let rect = CGRect(
            x: position.x - size / 2,
            y: position.y - size / 2,
            width: size,
            height: size
        )

        // Player circle
        let color = player.team == .home ? UIColor.blue : UIColor.red
        context.setFillColor(color.cgColor)
        context.fillEllipse(in: rect)

        // Border
        context.setStrokeColor(UIColor.white.cgColor)
        context.setLineWidth(2)
        context.strokeEllipse(in: rect)

        // Number
        let text = "\(player.number)" as NSString
        let attributes: [NSAttributedString.Key: Any] = [
            .font: UIFont.boldSystemFont(ofSize: size * 0.5),
            .foregroundColor: UIColor.white
        ]
        let textSize = text.size(withAttributes: attributes)
        let textRect = CGRect(
            x: position.x - textSize.width / 2,
            y: position.y - textSize.height / 2,
            width: textSize.width,
            height: textSize.height
        )
        text.draw(in: textRect, withAttributes: attributes)
    }
}
