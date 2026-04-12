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

/**
 * @file anaphase.hpp
 *
 * @authors SkyTheDragon
 *
 * Contains the anaphase of file parsing, which parses out the library structure, but does not fill out symbol data
 */
namespace quartz::lib::parser {
    namespace anaphase_errors {
        namespace document_problem {
            /**
             * @brief Error for when a library is missing the 'group' attribute
             */
            struct LibraryMissingGroup {
                /**
                 * Offending library's xml
                 */
                ::pugi::xml_node offender;
            };
            /**
             * @brief Error for when library's group is already in use
             */
            struct LibraryGroupAlreadyExists {
                /**
                 * offending library's xml
                 */
                ::pugi::xml_node offender;
                /**
                 * Name of the group the library is trying to take
                 */
                std::string group;
            };
            /**
             * @brief Error for when a folder is missing a name
             */
            struct FolderMissingName {
                /**
                 * Offending folder's xml
                 */
                ::pugi::xml_node offender;
            };
            /**
             * @brief Error for when a symbol is missing a name
             */
            struct SymbolMissingName {
                /**
                 * Offending symbol's xml
                 */
                ::pugi::xml_node offender;
            };
            /**
             * @brief Error for when folder's name is already in use
             */
            struct FolderNameTaken {
                /**
                 * Offending folder's xml
                 */
                ::pugi::xml_node offender;
                /**
                 * Name folder is attempting to take
                 */
                std::string name;
            };
            /**
             * @brief Error for when symbol's name is already in use
             */
            struct SymbolNameTaken {
                /**
                 * Offending symbol's xml
                 */
                ::pugi::xml_node offender;
                /**
                 * Name symbol is attempting to take
                 */
                std::string name;
            };
        }
        /**
         * Variant between all document problems
         */
        using InvalidDocument = ::std::variant<
            document_problem::LibraryMissingGroup,
            document_problem::LibraryGroupAlreadyExists,
            document_problem::FolderMissingName,
            document_problem::SymbolMissingName,
            document_problem::FolderNameTaken,
            document_problem::SymbolNameTaken>;
    }

    using AnaphaseError = ::std::vector<anaphase_errors::InvalidDocument>;

    /**
     * @brief Parses library structure into file
     * @param file File to load into
     * @param doc Document to parse
     * @return Nothing, or error
     */
    ::std::expected<void, AnaphaseError> anaphase(core::AnimFile& file, const ::pugi::xml_document& doc);

    /**
     * @brief Parses library's content from document
     * @param file File to load into
     * @param library XML of library being parsed
     * @param id Id of library being parsed
     * @return Nothing, or error
     */
    ::std::expected<void, AnaphaseError> anaphase_parse_library(
        core::AnimFile& file,
        ::pugi::xml_node library,
        core::LibraryId id);

    /**
     * @brief Parse folder's content from document
     * @param file File to load into
     * @param folder XML of folder being parsed
     * @param id Id of folder being parsed
     * @return Nothing or error
     */
    ::std::expected<void, AnaphaseError> anaphase_parse_folder(
        core::AnimFile& file,
        ::pugi::xml_node folder,
        core::FolderId id);
}
