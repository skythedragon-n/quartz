//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Jul 2026.
//

#pragma once
#include <vector>
#include <quartz/core/drawing_utils.hpp>

#include "id_sys.hpp"


namespace quartz::renderer::coesite {
    struct Node {
        core::Transform transform_;
        ::std::vector<NodeId> children_;
        ::std::vector<StrokeId> strokes_;
    };
}
