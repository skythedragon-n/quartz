//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#include "AnimatedLayer.hpp"

#include <utility>

namespace quartz::core {
    AnimatedLayer::AnimatedLayer(::std::string  name, const Color color) :
    name_(std::move(name)),
    color_(color)
    {}

    FrameContainer<FrameId>& AnimatedLayer::get_frames() {
        return frames_;
    }
}
