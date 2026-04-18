//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 18 Apr 2026.
//

#include "./Instance.hpp"

namespace quartz::core {
    Instance::Instance(IdKey, AnimFile*, SymbolId source, Transform transform, InstanceId) :
        source_(source),
        transform_(transform)
    {}

    SymbolId Instance::source() const {
        return source_;
    }

    void Instance::set_source(const SymbolId& source) {
        source_ = source;
    }

    Transform Instance::transform() const {
        return transform_;
    }

    void Instance::set_transform(const Transform& transform) {
        transform_ = transform;
    }
}
