//
//  PlayerView.swift
//  SoccerPlayPlanner
//
//  Draggable player piece
//

import SwiftUI

struct PlayerView: View {
    let player: Player
    let size: CGFloat
    var isSelected: Bool = false
    var onDragChanged: ((CGPoint) -> Void)?
    var onDragEnded: (() -> Void)?

    @State private var dragOffset: CGSize = .zero

    var body: some View {
        ZStack {
            // Player circle
            Circle()
                .fill(player.color)
                .overlay(
                    Circle()
                        .strokeBorder(isSelected ? Color.yellow : Color.white, lineWidth: isSelected ? 3 : 2)
                )
                .shadow(radius: 3)

            // Player number
            Text("\(player.number)")
                .font(.system(size: size * 0.5, weight: .bold))
                .foregroundColor(.white)
        }
        .frame(width: size, height: size)
        .gesture(
            DragGesture()
                .onChanged { value in
                    dragOffset = value.translation
                    onDragChanged?(value.location)
                }
                .onEnded { _ in
                    dragOffset = .zero
                    onDragEnded?()
                }
        )
        .offset(dragOffset)
    }
}

#Preview {
    VStack(spacing: 20) {
        PlayerView(
            player: Player(number: 10, team: .home, position: .zero),
            size: 50
        )

        PlayerView(
            player: Player(number: 9, team: .away, position: .zero),
            size: 50,
            isSelected: true
        )
    }
    .padding()
}
