//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 27 Jun 2026.
//

#pragma once

#include <quartz/core/drawing_utils.hpp>

#include "id_sys.hpp"

namespace quartz::renderer::coesite {
    struct Scene {
        NodeId root_{};
        core::Color background_color_;
        size_t x_resolution_ = 1920;
        size_t y_resolution_ = 1080;
        core::Transform camera_transform_;

        Scene(
            IdKey key,
            CoesiteRoot* file,
            NodeId root,
            core::Color background_color,
            size_t x_resolution,
            size_t y_resolution,
            const core::Transform& camera_transform,
            SceneId self
        );
    };
}
