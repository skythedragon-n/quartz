//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 20 Mar 2026.
//

#pragma once
#include <expected>
#include <string>
#include <variant>

#include <quartz/core/id_sys.hpp>
#include <pugixml.hpp>

namespace quartz::lib::parser {
    namespace anaphase_errors {
        namespace document_problem {
            struct LibraryMissingGroup {
                ::pugi::xml_node offender;
            };
            struct LibraryGroupAlreadyExists {
                ::pugi::xml_node offender;
                std::string group;
            };
            struct FolderMissingName {
                ::pugi::xml_node offender;
            };
            struct SymbolMissingName {
                ::pugi::xml_node offender;
            };
            struct FolderNameTaken {
                ::pugi::xml_node offender;
                std::string name;
            };
            struct SymbolNameTaken {
                ::pugi::xml_node offender;
                std::string name;
            };
        }
        using InvalidDocument = ::std::variant<
            document_problem::LibraryMissingGroup,
            document_problem::LibraryGroupAlreadyExists,
            document_problem::FolderMissingName,
            document_problem::SymbolMissingName,
            document_problem::FolderNameTaken,
            document_problem::SymbolNameTaken>;
    }

    using AnaphaseError = ::std::variant<anaphase_errors::InvalidDocument>;

    ::std::expected<void, AnaphaseError> anaphase(core::AnimFile& file, const ::pugi::xml_document& doc);

    ::std::expected<void, AnaphaseError> anaphase_parse_library(
        core::AnimFile& file,
        ::pugi::xml_node library,
        core::LibraryId id);

    ::std::expected<void, AnaphaseError> anaphase_parse_folder(
        core::AnimFile& file,
        ::pugi::xml_node folder,
        core::FolderId id);
}
