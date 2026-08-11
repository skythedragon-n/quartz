//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 27 Jun 2026.
//

#include "Scene.hpp"

namespace quartz::renderer::coesite {
    Scene::Scene(
        IdKey key,
        CoesiteRoot* file,
        NodeId root,
        const core::Color background_color,
        const size_t x_resolution,
        const size_t y_resolution,
        const core::Transform& camera_transform,
        SceneId self
    ) :
        root_(root),
        background_color_(background_color),
        x_resolution_(x_resolution),
        y_resolution_(y_resolution),
        camera_transform_(camera_transform)
    {}
}
