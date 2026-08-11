//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 29 Jun 2026.
//

#pragma once

#include "./id_sys.hpp"

namespace quartz::renderer::coesite {
    class CoesiteRoot {
    public:
        CoesiteRoot();

        IdContainer<Scene> scenes;
        IdContainer<Node> nodes;
        IdContainer<Stroke> strokes;
    };
}
