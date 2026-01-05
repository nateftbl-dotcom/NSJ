//
//  FieldView.swift
//  SoccerPlayPlanner
//
//  Renders a soccer field with proper proportions and markings
//

import SwiftUI

struct FieldView: View {
    let fieldColor = Color(red: 0.2, green: 0.6, blue: 0.2)
    let lineColor = Color.white

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Field background
                Rectangle()
                    .fill(fieldColor)

                // Field markings
                Canvas { context, size in
                    let lineWidth: CGFloat = 2.0
                    let path = Path { path in
                        // Outer boundary
                        path.addRect(CGRect(x: 0, y: 0, width: size.width, height: size.height))

                        // Halfway line
                        path.move(to: CGPoint(x: size.width / 2, y: 0))
                        path.addLine(to: CGPoint(x: size.width / 2, y: size.height))

                        // Center circle
                        let centerCircleRadius = size.height * 0.15
                        path.addEllipse(in: CGRect(
                            x: size.width / 2 - centerCircleRadius,
                            y: size.height / 2 - centerCircleRadius,
                            width: centerCircleRadius * 2,
                            height: centerCircleRadius * 2
                        ))

                        // Center spot
                        let centerSpotRadius: CGFloat = 3
                        path.addEllipse(in: CGRect(
                            x: size.width / 2 - centerSpotRadius,
                            y: size.height / 2 - centerSpotRadius,
                            width: centerSpotRadius * 2,
                            height: centerSpotRadius * 2
                        ))

                        // Left penalty area
                        let penaltyAreaWidth = size.width * 0.15
                        let penaltyAreaHeight = size.height * 0.6
                        path.addRect(CGRect(
                            x: 0,
                            y: (size.height - penaltyAreaHeight) / 2,
                            width: penaltyAreaWidth,
                            height: penaltyAreaHeight
                        ))

                        // Right penalty area
                        path.addRect(CGRect(
                            x: size.width - penaltyAreaWidth,
                            y: (size.height - penaltyAreaHeight) / 2,
                            width: penaltyAreaWidth,
                            height: penaltyAreaHeight
                        ))

                        // Left goal area
                        let goalAreaWidth = size.width * 0.05
                        let goalAreaHeight = size.height * 0.35
                        path.addRect(CGRect(
                            x: 0,
                            y: (size.height - goalAreaHeight) / 2,
                            width: goalAreaWidth,
                            height: goalAreaHeight
                        ))

                        // Right goal area
                        path.addRect(CGRect(
                            x: size.width - goalAreaWidth,
                            y: (size.height - goalAreaHeight) / 2,
                            width: goalAreaWidth,
                            height: goalAreaHeight
                        ))

                        // Left penalty spot
                        let penaltySpotDistance = size.width * 0.10
                        let spotRadius: CGFloat = 3
                        path.addEllipse(in: CGRect(
                            x: penaltySpotDistance - spotRadius,
                            y: size.height / 2 - spotRadius,
                            width: spotRadius * 2,
                            height: spotRadius * 2
                        ))

                        // Right penalty spot
                        path.addEllipse(in: CGRect(
                            x: size.width - penaltySpotDistance - spotRadius,
                            y: size.height / 2 - spotRadius,
                            width: spotRadius * 2,
                            height: spotRadius * 2
                        ))

                        // Left penalty arc
                        let arcRadius = size.height * 0.15
                        let leftArcCenter = CGPoint(x: penaltySpotDistance, y: size.height / 2)
                        path.addArc(
                            center: leftArcCenter,
                            radius: arcRadius,
                            startAngle: Angle(degrees: -60),
                            endAngle: Angle(degrees: 60),
                            clockwise: false
                        )

                        // Right penalty arc
                        let rightArcCenter = CGPoint(x: size.width - penaltySpotDistance, y: size.height / 2)
                        path.addArc(
                            center: rightArcCenter,
                            radius: arcRadius,
                            startAngle: Angle(degrees: 120),
                            endAngle: Angle(degrees: 240),
                            clockwise: false
                        )

                        // Corner arcs
                        let cornerRadius: CGFloat = 15
                        // Top-left
                        path.addArc(
                            center: CGPoint(x: 0, y: 0),
                            radius: cornerRadius,
                            startAngle: Angle(degrees: 0),
                            endAngle: Angle(degrees: 90),
                            clockwise: false
                        )
                        // Top-right
                        path.addArc(
                            center: CGPoint(x: size.width, y: 0),
                            radius: cornerRadius,
                            startAngle: Angle(degrees: 90),
                            endAngle: Angle(degrees: 180),
                            clockwise: false
                        )
                        // Bottom-left
                        path.addArc(
                            center: CGPoint(x: 0, y: size.height),
                            radius: cornerRadius,
                            startAngle: Angle(degrees: 270),
                            endAngle: Angle(degrees: 360),
                            clockwise: false
                        )
                        // Bottom-right
                        path.addArc(
                            center: CGPoint(x: size.width, y: size.height),
                            radius: cornerRadius,
                            startAngle: Angle(degrees: 180),
                            endAngle: Angle(degrees: 270),
                            clockwise: false
                        )
                    }

                    context.stroke(
                        path,
                        with: .color(.white),
                        lineWidth: lineWidth
                    )
                }
            }
        }
        .aspectRatio(1.5, contentMode: .fit) // Soccer field is typically 1.5:1 ratio
    }
}

#Preview {
    FieldView()
        .padding()
}
