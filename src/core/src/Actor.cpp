//
// Created by xerox on 3/29/2026.
//

#include <Actor.h>
#include <Utils.h>

namespace core {
    void Actor::draw() const {
        // TODO: don't draw when outside camera
        m_pos.DrawCircle(32, WHITE);
    }

    void Actor::drawDebug() const {
        // Draw Chunk
        const auto chunkPos = snapToChunk(m_pos);
        DrawRectangleLines(chunkPos.x, chunkPos.y, CHUNK_SIZE, CHUNK_SIZE, RED);
    }
}