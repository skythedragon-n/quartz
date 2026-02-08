//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once
#include <vector>

#include "Drawing.hpp"

namespace quartz::renderer {
    class Stroke {
        ::std::vector<BezierSection> points_;
        double thickness_;
        Color color_;

    public:
        Stroke(double thickness, Color color);

        void add_point(Point point);

        void set_thickness(double thickness);
        void set_color(Color color);

        [[nodiscard]] double thickness() const {return thickness_;}
        [[nodiscard]] Color color() const {return color_;}
        [[nodiscard]] const ::std::vector<BezierSection>& points() const {return points_;}
    };
}
