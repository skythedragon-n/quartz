//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 20 Mar 2026.
//

#include "./anaphase.hpp"

#include <quartz/core/AnimFile.hpp>

namespace quartz::lib::parser {
    ::std::expected<void, AnaphaseError> anaphase(core::AnimFile& file, const ::pugi::xml_document& doc) {
        for (const ::pugi::xml_node library_node : doc.child("quartz_document").children("library")) {
            const ::pugi::xml_attribute group = library_node.attribute("group");

            if (!group) {
                return ::std::unexpected(anaphase_errors::document_problem::LibraryMissingGroup{library_node});
            }

            auto add_res = file.add_library(group.as_string());

            if (!add_res) {
                return ::std::unexpected(
                    anaphase_errors::document_problem::LibraryGroupAlreadyExists{
                        library_node,
                        group.as_string()});
            }

            core::LibraryId library_id = *add_res;

            auto library_res = anaphase_parse_library(file, library_node, library_id);
        }

        return {};
    }
}
