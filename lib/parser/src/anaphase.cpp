//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 20 Mar 2026.
//

#include "./anaphase.hpp"

#include <quartz/core/AnimFile.hpp>

namespace quartz::lib::parser {
    ::std::expected<void, AnaphaseError> anaphase(core::AnimFile& file, ::pugi::xml_document& doc) {
        for (::pugi::xml_node library : doc.child("quartz_document").children("library")) {
            ::pugi::xml_attribute group = library.attribute("group");

            if (!group) {
                return ::std::unexpected(anaphase_errors::document_problem::LibraryMissingGroup{});
            }

            auto add_res = file.add_library(group.as_string());

            if (!add_res) {
                return ::std::unexpected(
                    anaphase_errors::document_problem::LibraryGroupAlreadyExists{group.as_string()});
            }

            //TODO: parse library content
        }

        return {};
    }
}
