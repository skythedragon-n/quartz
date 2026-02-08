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
        //TODO: implement
    }

    void Stroke::set_color(Color color) {
        color_ = color;
    }

    void Stroke::set_thickness(double thickness) {
        thickness_ = thickness;
    }
}
