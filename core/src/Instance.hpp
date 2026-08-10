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
    /**
     * @class Instance
     * @authors SkyTheDragon
     *
     * @brief Represents an instance of a symbol.
     */
    class Instance {
        SymbolId source_;
        Transform transform_;

    public:
        Instance(IdKey, AnimFile*, SymbolId source, Transform transform, InstanceId);

        /**
         * @brief Sets the source Symbol of the instance
         * @return Source Symbol
         */
        [[nodiscard]] SymbolId source() const;

        /**
         * @brief Sets the source symbol of the instance. Make sure to also handle the symbol's refcounts
         * @param source Value to set
         */
        void set_source(const SymbolId& source);

        /**
         * @brief Gets the transform the instance applies to the instanced symbol
         * @return Transform of instance
         */
        [[nodiscard]] Transform transform() const;

        /**
         * @brief Sets transform of the instance
         * @param transform New value for transform
         */
        void set_transform(const Transform& transform);
    };
}
