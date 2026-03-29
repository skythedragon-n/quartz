//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 20 Mar 2026.
//

#include "./anaphase.hpp"

#include <quartz/core/AnimFile.hpp>
#include <quartz/lib/core_lib/library_system.hpp>
#include <qtil/panic.hpp>

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

    ::std::expected<void, AnaphaseError> anaphase_parse_library(
        core::AnimFile& file,
        const ::pugi::xml_node library,
        core::LibraryId id) {
        auto library_res = file.libraries.resolve(id);

        if (!library_res) {
            ::qtil::panic("Library just added now deleted. This is bad");
        }

        core::Library* library_p = *library_res;

        for (const ::pugi::xml_node folder_node : library.children("folder")) {
            const ::pugi::xml_attribute name = folder_node.attribute("name");

            if (!name) {
                return ::std::unexpected(
                    anaphase_errors::document_problem::FolderMissingName{folder_node});
            }

            if (!library_p->available(name.as_string())) {
                return ::std::unexpected(anaphase_errors::document_problem::FolderNameTaken{
                    folder_node,
                    name.as_string()});
            }

            auto add_res = core_lib::add_folder(file, id, name.as_string());

            if (!add_res) {
                return ::std::unexpected(anaphase_errors::document_problem::FolderNameTaken{
                    folder_node,
                    name.as_string()
                });
            }

            core::FolderId folder_id = *add_res;

            auto parse_res = anaphase_parse_folder(file, folder_node, folder_id);

            if (!parse_res) {
                return ::std::unexpected(parse_res.error());
            }
        }

        for (const ::pugi::xml_node symbol_node : library.children("symbol")) {
            const ::pugi::xml_attribute name = symbol_node.attribute("name");

            if (!name) {
                return ::std::unexpected(
                    anaphase_errors::document_problem::SymbolMissingName{symbol_node});
            }

            if (!library_p->available(name.as_string())) {
                return ::std::unexpected(anaphase_errors::document_problem::SymbolNameTaken{
                    symbol_node,
                    name.as_string()});
            }

            auto add_res = core_lib::add_symbol(file, id, name.as_string());

            if (!add_res) {
                return ::std::unexpected(anaphase_errors::document_problem::SymbolNameTaken{
                    symbol_node,
                    name.as_string()
                });
            }
        }

        return {};
    }

    ::std::expected<void, AnaphaseError> anaphase_parse_folder(
        core::AnimFile& file,
        const ::pugi::xml_node folder,
        core::FolderId id) {
        auto folder_res = file.folders.resolve(id);

        if (!folder_res) {
            ::qtil::panic("Folder just added now deleted. This is bad");
        }

        core::LibraryFolder* folder_p = *folder_res;

        for (const ::pugi::xml_node folder_node : folder.children("folder")) {
            const ::pugi::xml_attribute name = folder_node.attribute("name");

            if (!name) {
                return ::std::unexpected(
                    anaphase_errors::document_problem::FolderMissingName{folder_node});
            }

            if (!folder_p->available(name.as_string())) {
                return ::std::unexpected(anaphase_errors::document_problem::FolderNameTaken{
                    folder_node,
                    name.as_string()
                });
            }

            auto add_res = core_lib::add_folder(file, id, name.as_string());

            if (!add_res) {
                return ::std::unexpected(anaphase_errors::document_problem::FolderNameTaken{
                    folder_node,
                    name.as_string()
                });
            }

            core::FolderId folder_id = *add_res;

            auto parse_res = anaphase_parse_folder(file, folder_node, folder_id);

            if (!parse_res) {
                return ::std::unexpected(parse_res.error());
            }
        }

        for (const ::pugi::xml_node symbol_node : folder.children("symbol")) {
            const ::pugi::xml_attribute name = symbol_node.attribute("name");

            if (!name) {
                return ::std::unexpected(
                    anaphase_errors::document_problem::SymbolMissingName{symbol_node});
            }

            if (!folder_p->available(name.as_string())) {
                return ::std::unexpected(anaphase_errors::document_problem::SymbolNameTaken{
                    symbol_node,
                    name.as_string()
                });
            }

            auto add_res = core_lib::add_symbol(file, id, name.as_string());

            if (!add_res) {
                return ::std::unexpected(anaphase_errors::document_problem::SymbolNameTaken{
                    symbol_node,
                    name.as_string()
                });
            }
        }

        return {};
    }
}
