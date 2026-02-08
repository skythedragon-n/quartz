//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <cstdint>
#include <vector>

#include "Stroke.hpp"

namespace quartz::renderer {

    class Drawing {
        std::vector<Stroke> strokes_;

    public:
        void add_stroke(Color color, double thickness, std::vector<Point> points);
        Stroke& get_stroke(size_t index);
        ::std::vector<Stroke>& strokes() { return strokes_; }
    };
}
