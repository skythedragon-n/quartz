//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#include "AnimatedLayer.hpp"

#include <utility>

namespace quartz::core {
    AnimatedLayer::AnimatedLayer(IdKey, AnimFile*, ::std::string name, Color color, SymbolId parent, AnimLayerId) :
    name_(std::move(name)),
    color_(color),
    parent_(parent)
    {}

    FrameContainer<FrameId>& AnimatedLayer::get_frames() {
        return frames_;
    }
}
