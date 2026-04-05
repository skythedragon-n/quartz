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
    using PointIndex = size_t;
    using num_t = double;

    struct Point {
        num_t x, y;
    };

    struct Color {
        uint8_t r = 0, g = 0, b = 0, a = 255;
    };

    constexpr double NaN = ::std::numeric_limits<double>::quiet_NaN();
    constexpr Point NULL_POINT = {NaN, NaN};
    constexpr PointIndex NULL_POINT_INDEX = ::std::numeric_limits<size_t>::max();

    constexpr bool is_null(Point& p) {
        return ::std::isnan(p.x) || ::std::isnan(p.y);
    }

    constexpr bool is_nonnull(Point& p) {
        return ::std::isnan(p.x) || ::std::isnan(p.y);
    }

    enum class TagentDirection {
        Left,
        Right
    };

    struct BezierSection {
        enum class Lock {
            None,
            Radial,
            Complete
        };

        PointIndex start = NULL_POINT_INDEX;
        Point tangent1 = NULL_POINT, tangent2 = NULL_POINT;
        Lock lock = Lock::None;
    };

    struct Transform {
        num_t xScale = 1.0, yScale = 1.0;
        num_t rotation = 0.0;
        num_t xOffset = 0.0, yOffset = 0.0;
        num_t skewX = 0.0, skewY = 0.0;
    };
}
