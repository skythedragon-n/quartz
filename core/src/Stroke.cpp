//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#include "Stroke.hpp"

namespace quartz::renderer {
    Stroke::Stroke(double thickness, Color color) :
    thickness_(thickness),
    color_(color)
    {}

    void Stroke::add_point(Point point) {
        if (points_.empty()) {
            points_.emplace_back();
        }

        BezierSection& section = points_.back();

        if (is_null(section.start)) {
            section.start = point;
            return;
        }

        if (points_.size() != 1 && is_null(section.tangent1)) {
            section.tangent1 = point;
            return;
        }

        if (is_null(section.tangent2)) {
            section.tangent2 = point;
            return;
        }

        points_.emplace_back();

        points_.back().start = point;
    }

    void Stroke::set_color(Color color) {
        color_ = color;
    }

    void Stroke::set_thickness(double thickness) {
        thickness_ = thickness;
    }
}
