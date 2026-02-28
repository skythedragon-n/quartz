//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once

#include <cstdint>
#include <cmath>
#include <limits>

namespace quartz::core {
    struct Point {
        double x, y;
    };

    struct Color {
        uint8_t r = 0, g = 0, b = 0, a = 255;
    };

    constexpr double NaN = ::std::numeric_limits<double>::quiet_NaN();
    constexpr Point null_point = {NaN, NaN};

    constexpr bool is_null(Point& p) {
        return ::std::isnan(p.x) || ::std::isnan(p.y);
    }

    constexpr bool is_nonnull(Point& p) {
        return ::std::isnan(p.x) || ::std::isnan(p.y);
    }

    struct BezierSection {
        enum class Lock {
            None,
            Radial,
            Complete
        };

        Point start = null_point;
        Point tangent1 = null_point, tangent2 = null_point;
        Lock lock = Lock::None;
    };

    struct Transform {
        double xScale = 1.0, yScale = 1.0;
        double rotation = 0.0;
        double xOffset = 0.0, yOffset = 0.0;
        double skewX = 0.0, skewY = 0.0;
    };
}
