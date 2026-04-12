//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Mar 2026.
//

#pragma once

#include <quartz/core/AnimFile.hpp>
#include <pugixml.hpp>

namespace quartz::lib::parser {
    enum class MetaphaseError {
        InvalidDocument,
        SpecTooNew,
    };

    /**
     * @brief Parses in metadata
     * @param file File to perform operations on
     * @param doc Document to parse
     * @return Nothing, or error
     */
    ::std::expected<void, MetaphaseError> metaphase(core::AnimFile& file, ::pugi::xml_document& doc);
}
