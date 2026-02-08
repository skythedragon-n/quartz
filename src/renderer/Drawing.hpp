//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <cstdint>
#include <vector>

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

    class Drawing {
        std::vector<Stroke> strokes_;

    public:
        Drawing();
    };
}
