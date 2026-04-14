//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Mar 2026.
//

#pragma once

#include <expected>
#include <pugixml.hpp>

/**
 * @file prophase.hpp
 *
 * @authors SkyTheDragon
 *
 * Parses file XML into an XML document. The prophase of file parsing
 */
namespace quartz::lib::parser {
    ::std::expected<void, ::pugi::xml_parse_result> prophase(::pugi::xml_document& doc, const ::std::string& path);
}
