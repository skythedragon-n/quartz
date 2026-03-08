//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#include "Frame.hpp"

#include <utility>

namespace quartz::core {
    using namespace frame_types;

    Frame::Frame(std::variant<Empty, Normal> data) :
    data_(std::move(data))
    {}

    Frame Frame::empty() {
        return Frame(Empty());
    }

    Frame Frame::normal(Drawing drawing) {
        return Frame(Normal{{}, std::move(drawing)});
    }
}
