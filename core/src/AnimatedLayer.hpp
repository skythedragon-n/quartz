//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once
#include <string>
#include <vector>

#include "drawing_utils.hpp"
#include "id_sys.hpp"
#include "quartz/core/FrameContainer.hpp"

namespace quartz::core {
    class AnimatedLayer {
        FrameContainer<FrameId> frames_;

        ::std::string name_;
        Color color_;
        SymbolId parent_;

    public:

        AnimatedLayer(IdKey, AnimFile*, ::std::string name, Color color, SymbolId parent, AnimLayerId);

        /**
         * @brief Return reference to frames in the layer
         * @return Reference to frames
         */
        FrameContainer<FrameId>& get_frames();
    };
}
