//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Drawing.hpp"

namespace quartz::renderer {
    void Drawing::add_stroke(Color color, double thickness, std::vector<Point> points) {
        strokes_.emplace_back(thickness, color);

        for (auto point : points) {
            strokes_.rbegin()->add_point(point);
        }
    }

    Stroke& Drawing::get_stroke(size_t index) {
        return strokes_[index];
    }
}
