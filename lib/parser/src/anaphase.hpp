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
        }
        using InvalidDocument = ::std::variant<
            document_problem::LibraryMissingGroup,
            document_problem::LibraryGroupAlreadyExists>;
    }

    using AnaphaseError = ::std::variant<anaphase_errors::InvalidDocument>;

    ::std::expected<void, AnaphaseError> anaphase(core::AnimFile& file, const ::pugi::xml_document& doc);
}
