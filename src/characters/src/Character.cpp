//
// Created by xerox on 5/16/2026.
//

#include <Character.h>
#include <Utils.h>

void Character::update() {
    // Check which geometry we're in
    const auto delta = m_pos + m_vel;
    m_fallingVel = FALLING_VELOCITY;
    for (const auto &g : m_map->m_geometries) {
        if (CheckCollisionPointPoly(m_pos, g.m_vertices.data(), g.m_vertices.size())) {
            // This is the one
            m_fallingVel = 0;

            // Check collision with each line
            for (const auto &[a, b] : g.m_collisionLines) {
                const auto result = linesCollisionPoint(m_pos, delta, a, b);
                if (result.has_value()) {
                    m_vel = 0;
                }
            }
        }
    }

    if (m_fallingVel > 0) {
        m_pos.y += m_fallingVel;
        m_vel = 0;
    } else {
        m_animationBank[m_animationIndex].update();
        m_pos += m_vel;
        m_vel /= 1.15;
    }
}

void Character::draw() const {
    // // getCenter().DrawCircle(32, WHITE);
    // m_assetManager->getTex(m_sheetTex).Draw(
    //     raylib::Rectangle{0, getAngleIndex() * m_size.y, m_size.x, m_size.y},
    //     raylib::Rectangle{m_pos, m_size},
    //     m_drawOrigin
    // );
    m_animationBank[m_animationIndex].draw(m_pos, getAngleIndex());
}

void Character::drawDebug() const {
    m_pos.DrawLine(m_pos + getVector() * 32, BLUE);
    m_pos.DrawLine(m_pos + m_vel, 2, LIME);
}

void Character::switchAnimation(const int animationIndex) {
    m_animationBank[m_animationIndex].reset();
    m_animationIndex = animationIndex;
}

float Character::getAngleIndex() const {
    switch (m_direction) {
        case Direction::Right:
            return 0;
        case Direction::DownRight:
            return 1;
        case Direction::Down:
            return 2;
        case Direction::DownLeft:
            return 3;
        case Direction::Left:
            return 4;
        case Direction::UpLeft:
            return 5;
        case Direction::Up:
            return 6;
        case Direction::UpRight:
            return 7;
    }

    return 0;
}

float Character::getAngle() const {
    switch (m_direction) {
        case Direction::Right:
            return 0;
        case Direction::DownRight:
            return PI_Q;
        case Direction::Down:
            return PI_Q * 2;
        case Direction::DownLeft:
            return PI_Q * 3;
        case Direction::Left:
            return PI_Q * 4;
        case Direction::UpLeft:
            return PI_Q * 5;
        case Direction::Up:
            return PI_Q * 6;
        case Direction::UpRight:
            return PI_Q * 7;
    }

    return 0;
}

raylib::Vector2 Character::getVector() const {
    switch (m_direction) {
        case Direction::Right:
            return {1, 0};
        case Direction::DownRight:
            return {1, 1};
        case Direction::Down:
            return {0, 1};
        case Direction::DownLeft:
            return {-1, 1};
        case Direction::Left:
            return {-1, 0};
        case Direction::UpLeft:
            return {-1, -1};
        case Direction::Up:
            return {0, -1};
        case Direction::UpRight:
            return {1, -1};
    }

    return {};
}
