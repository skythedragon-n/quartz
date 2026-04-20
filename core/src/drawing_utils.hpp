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

/**
 * @file drawing_utils.hpp
 *
 * @authors SkyTheDragon
 *
 * Various utilities for drawings & storing drawing data
 */
namespace quartz::core {
    /**
     * @brief A number type alias for numbers storing drawing info
     */
    using num_t = double;

    /**
     * @brief Parses a num_t, however a num_t needs to be parsed
     * @param str String to parse a number from
     * @return Parsed number
     */
    inline num_t parse_num(const ::std::string& str) {
        return ::std::stod(str);
    }

    /**
     * @brief A point in 2D space
     */
    struct Point {
        num_t x, y;
    };

    /**
     * @brief A 32-bit RGBA color
     */
    struct Color {
        uint8_t r = 0, g = 0, b = 0, a = 255;
    };

    constexpr double NaN = ::std::numeric_limits<double>::quiet_NaN();
    constexpr Point NULL_POINT = {NaN, NaN};
    constexpr size_t NULL_POINT_INDEX = ::std::numeric_limits<size_t>::max();

    constexpr bool is_null(Point& p) {
        return ::std::isnan(p.x) || ::std::isnan(p.y);
    }

    constexpr bool is_nonnull(Point& p) {
        return !is_null(p);
    }

    enum class CornerType {
        Bevel,
        Round,
        Miter
    };

    enum class TagentDirection {
        Lastwise,
        Nextwise
    };

    struct BezierSection {
        enum class Lock {
            None,
            Radial,
            Complete
        };

        size_t start = NULL_POINT_INDEX;
        Point lastwise_tangent = NULL_POINT, nextwise_tangent = NULL_POINT;
        Lock lock = Lock::None;
    };

    struct Transform {
        num_t xScale = 1.0, yScale = 1.0;
        num_t rotation = 0.0;
        num_t xOffset = 0.0, yOffset = 0.0;
        num_t skewX = 0.0, skewY = 0.0;
    };
}
