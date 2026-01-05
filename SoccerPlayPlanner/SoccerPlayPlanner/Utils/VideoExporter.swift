//
//  VideoExporter.swift
//  SoccerPlayPlanner
//
//  Export animations to video format
//

import Foundation
import AVFoundation
import UIKit
import SwiftUI

class VideoExporter {
    static func exportToVideo(
        project: Project,
        size: CGSize = CGSize(width: 1920, height: 1080),
        fps: Int = 60,
        completion: @escaping (Result<URL, Error>) -> Void
    ) {
        let outputURL = FileManager.default.temporaryDirectory
            .appendingPathComponent("\(project.name)_\(UUID().uuidString).mp4")

        // Remove existing file if it exists
        try? FileManager.default.removeItem(at: outputURL)

        guard let videoWriter = try? AVAssetWriter(outputURL: outputURL, fileType: .mp4) else {
            completion(.failure(ExportError.failedToCreateWriter))
            return
        }

        let videoSettings: [String: Any] = [
            AVVideoCodecKey: AVVideoCodecType.h264,
            AVVideoWidthKey: size.width,
            AVVideoHeightKey: size.height
        ]

        let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
        let adaptor = AVAssetWriterInputPixelBufferAdaptor(
            assetWriterInput: writerInput,
            sourcePixelBufferAttributes: [
                kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_32ARGB,
                kCVPixelBufferWidthKey as String: size.width,
                kCVPixelBufferHeightKey as String: size.height
            ]
        )

        videoWriter.add(writerInput)

        guard videoWriter.startWriting() else {
            completion(.failure(ExportError.failedToStartWriting))
            return
        }

        videoWriter.startSession(atSourceTime: .zero)

        let frameTime = CMTime(value: 1, timescale: CMTimeScale(fps))
        var frameCount: Int64 = 0
        let totalFrames = Int64(project.duration * Double(fps))

        let queue = DispatchQueue(label: "com.soccerplanner.videoexport")

        writerInput.requestMediaDataWhenReady(on: queue) {
            while writerInput.isReadyForMoreMediaData && frameCount < totalFrames {
                let currentTime = Double(frameCount) / Double(fps)
                let presentationTime = CMTime(value: frameCount, timescale: CMTimeScale(fps))

                // Generate frame for current time
                if let pixelBuffer = self.generateFrame(
                    for: project,
                    at: currentTime,
                    size: size
                ) {
                    adaptor.append(pixelBuffer, withPresentationTime: presentationTime)
                }

                frameCount += 1
            }

            if frameCount >= totalFrames {
                writerInput.markAsFinished()
                videoWriter.finishWriting {
                    if videoWriter.status == .completed {
                        completion(.success(outputURL))
                    } else {
                        completion(.failure(ExportError.writingFailed))
                    }
                }
            }
        }
    }

    private static func generateFrame(
        for project: Project,
        at time: TimeInterval,
        size: CGSize
    ) -> CVPixelBuffer? {
        let playerPositions = AnimationInterpolator.interpolatePositions(
            keyframes: project.keyframes,
            atTime: time
        )

        let renderer = UIGraphicsImageRenderer(size: size)
        let image = renderer.image { context in
            // Draw field
            drawField(in: context.cgContext, size: size)

            // Draw players
            for player in project.allPlayers {
                if let position = playerPositions[player.id] {
                    let screenPosition = CGPoint(
                        x: position.x * size.width,
                        y: position.y * size.height
                    )
                    drawPlayer(player, at: screenPosition, in: context.cgContext, size: 40)
                }
            }
        }

        return image.pixelBuffer()
    }

    private static func drawField(in context: CGContext, size: CGSize) {
        // Draw field background
        context.setFillColor(UIColor(red: 0.2, green: 0.6, blue: 0.2, alpha: 1.0).cgColor)
        context.fill(CGRect(origin: .zero, size: size))

        // Draw field lines (simplified)
        context.setStrokeColor(UIColor.white.cgColor)
        context.setLineWidth(3)

        // Outer boundary
        context.stroke(CGRect(origin: .zero, size: size))

        // Halfway line
        context.move(to: CGPoint(x: size.width / 2, y: 0))
        context.addLine(to: CGPoint(x: size.width / 2, y: size.height))
        context.strokePath()

        // Center circle
        let centerCircleRadius = size.height * 0.15
        context.addEllipse(in: CGRect(
            x: size.width / 2 - centerCircleRadius,
            y: size.height / 2 - centerCircleRadius,
            width: centerCircleRadius * 2,
            height: centerCircleRadius * 2
        ))
        context.strokePath()
    }

    private static func drawPlayer(_ player: Player, at position: CGPoint, in context: CGContext, size: CGFloat) {
        let rect = CGRect(
            x: position.x - size / 2,
            y: position.y - size / 2,
            width: size,
            height: size
        )

        // Draw player circle
        let color = player.team == .home ? UIColor.blue : UIColor.red
        context.setFillColor(color.cgColor)
        context.fillEllipse(in: rect)

        // Draw border
        context.setStrokeColor(UIColor.white.cgColor)
        context.setLineWidth(2)
        context.strokeEllipse(in: rect)

        // Draw number
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

    enum ExportError: Error {
        case failedToCreateWriter
        case failedToStartWriting
        case writingFailed
    }
}

// Extension to convert UIImage to CVPixelBuffer
extension UIImage {
    func pixelBuffer() -> CVPixelBuffer? {
        let width = Int(size.width)
        let height = Int(size.height)

        let attributes = [
            kCVPixelBufferCGImageCompatibilityKey: kCFBooleanTrue,
            kCVPixelBufferCGBitmapContextCompatibilityKey: kCFBooleanTrue
        ] as CFDictionary

        var pixelBuffer: CVPixelBuffer?
        let status = CVPixelBufferCreate(
            kCFAllocatorDefault,
            width,
            height,
            kCVPixelFormatType_32ARGB,
            attributes,
            &pixelBuffer
        )

        guard status == kCVReturnSuccess, let buffer = pixelBuffer else {
            return nil
        }

        CVPixelBufferLockBaseAddress(buffer, [])
        defer { CVPixelBufferUnlockBaseAddress(buffer, []) }

        let pixelData = CVPixelBufferGetBaseAddress(buffer)
        let rgbColorSpace = CGColorSpaceCreateDeviceRGB()

        guard let context = CGContext(
            data: pixelData,
            width: width,
            height: height,
            bitsPerComponent: 8,
            bytesPerRow: CVPixelBufferGetBytesPerRow(buffer),
            space: rgbColorSpace,
            bitmapInfo: CGImageAlphaInfo.noneSkipFirst.rawValue
        ) else {
            return nil
        }

        context.translateBy(x: 0, y: CGFloat(height))
        context.scaleBy(x: 1, y: -1)

        UIGraphicsPushContext(context)
        draw(in: CGRect(x: 0, y: 0, width: width, height: height))
        UIGraphicsPopContext()

        return buffer
    }
}
