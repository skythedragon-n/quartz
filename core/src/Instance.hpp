//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once
#include "Symbol.hpp"
#include "drawing_utils.hpp"


namespace quartz::core {
    struct Instance {
        SymbolId source;
        Transform transform_;
    };
}
