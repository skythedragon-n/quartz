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
    class Instance {
        SymbolId source_;
        Transform transform_;

    public:
        Instance(IdKey, AnimFile*, SymbolId source, Transform transform, InstanceId);

        [[nodiscard]] SymbolId source() const;

        void set_source(const SymbolId& source);

        [[nodiscard]] Transform transform() const;

        void set_transform(const Transform& transform);
    };
}
