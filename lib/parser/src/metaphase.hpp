//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Mar 2026.
//

#pragma once

#include <quartz/core/AnimFile.hpp>
#include <pugixml.hpp>

/**
 * @file metaphase.hpp
 *
 * @authors SkyTheDragon
 *
 * This file contains the code for parsing the metadata out of a QAD document, the metaphase of file parsing
 */
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
