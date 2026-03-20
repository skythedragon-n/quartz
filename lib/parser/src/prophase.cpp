//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Mar 2026.
//

#include "prophase.hpp"

#include <filesystem>

namespace quartz::lib::parser {
    ::std::expected<void, ::pugi::xml_parse_result> prophase(::pugi::xml_document& doc, const ::std::string& path) {
        auto result = doc.load_file(path.c_str());

        if (!result) {
            return ::std::unexpected(result);
        }

        return {};
    }
}
