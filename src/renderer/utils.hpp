//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once

#include <cstdint>

namespace quartz::renderer {
    struct Point {
        double x, y;
    };

    struct Color {
        uint8_t r, g, b, a;
    };

    struct BezierSection {
        Point start;
        Point tangent1, tangent2;
    };
}
