//
// Created by Jehok on 6/28/2026.
//

#include "UI.h"
#include "Combat.h"

combat::UI::UI() {
    m_aggroState = 100;
    m_rotaryPositon = 0;
    m_playerPos = raylib::Vector2();
}

void combat::UI::handleUI() {
    if (m_aggroState > 0) {
       m_aggroState -= m_AGGRO_DECEL_RATE;
    }
}
// Mysterious Wind
void combat::UI::drawUI(AssetManager* assets) {
    Rectangle combatRec;
    const int BAR_WIDTH = 100;

    combatRec.width = 1052;
    combatRec.height = 450;
    combatRec.x = 50;
    combatRec.y = 50;

    Vector2 points[4] = {
        {100, 10}, // top left
        {100, 50}, // bottom left
        {200, 50}, // bottom right
        {200,10}, // top right
    };

    DrawTriangleFan(points, 4, WHITE);

    DrawRectangleLinesEx(combatRec, 1.0f, WHITE);
}