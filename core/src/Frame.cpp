//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#include "Frame.hpp"

#include <utility>

namespace quartz::core {
    Frame::Frame(IdKey, AnimFile*, const FrameData& data, FrameId) :
    data_(data)
    {}

    Frame::Frame(IdKey, AnimFile*, FrameId) :
    data_(frame_types::Empty{})
    {}
}
