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

    ::std::expected<void, MetaphaseError> metaphase(core::AnimFile& file, ::pugi::xml_document& doc);
}
