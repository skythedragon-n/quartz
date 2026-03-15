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

        /**
         * @brief Gets layer name
         * @return Name of layer
         */
        [[nodiscard]] std::string name() const {
            return name_;
        }

        /**
         * @brief Gets layer color
         * @return Layer color
         */
        [[nodiscard]] Color color() const {
            return color_;
        }

        /**
         * @brief Gets layer's parent Symbol
         * @return Layer's parent
         */
        [[nodiscard]] SymbolId parent() const {
            return parent_;
        }

        /**
         * @brief Sets layer's name
         * @param name New name for layer
         */
        void set_name(const std::string& name) {
            name_ = name;
        }

        /**
         * @brief Sets Layer's color
         * @param color New color for layer
         */
        void set_color(const Color& color) {
            color_ = color;
        }
    };
}
