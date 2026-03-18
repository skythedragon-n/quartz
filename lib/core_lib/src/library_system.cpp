//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Feb 2026.
//

#include "library_system.hpp"

#include <quartz/core/AnimFile.hpp>
#include <quartz/core/LibraryFolder.hpp>

#include "../../../qtil/src/panic.hpp"
#include "qtil/vector_util.hpp"

namespace quartz::lib {
    ::std::expected<core::SymbolId, std::variant<core::FindFailure, core::ResolveFailure>> find_symbol_in_folder(
        core::AnimFile& file,
        core::FolderId folder_id,
        const ::std::string& path)
    {
        auto res = file.folders.resolve(folder_id);

        if (!res.has_value()) {
            return ::std::unexpected(res.error());
        }

        core::LibraryFolder* folder_p = res.value();

        ::std::size_t first_sep = path.find_first_of('/');

        if (first_sep == ::std::string::npos) {
            auto output = folder_p->find_symbol(path);

            if (!output) {
                return ::std::unexpected(core::FindFailure::NoSuchPath);
            }

            return *output;
        }

        auto subfolder = folder_p->find_folder(path.substr(0, first_sep));

        if (!subfolder) {
            return ::std::unexpected(core::FindFailure::NoSuchPath);
        }

        const core::FolderId folder = *subfolder;

        return find_symbol_in_folder(file, folder, path.substr(first_sep + 1));
    }

    ::std::expected<core::FolderId, ::std::variant<core::FindFailure, core::ResolveFailure>> find_folder_in_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& path)
    {
        auto res = file.folders.resolve(folder);

        if (!res.has_value()) {
            return ::std::unexpected(res.error());
        }

        core::LibraryFolder* folder_p = res.value();

        ::std::size_t first_sep = path.find_first_of('/');

        if (first_sep == ::std::string::npos) {
            auto output = folder_p->find_folder(path);

            if (!output) {
                return ::std::unexpected(core::FindFailure::NoSuchPath);
            }

            return *output;
        }

        auto subfolder = folder_p->find_folder(path.substr(0, first_sep));

        if (!subfolder) {
            return ::std::unexpected(core::FindFailure::NoSuchPath);
        }

        const core::FolderId out_folder = *subfolder;

        return find_folder_in_folder(file, out_folder, path.substr(first_sep + 1));
    }

    ::std::expected<core::FolderId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_folder(
        core::AnimFile& file,
        core::FolderId parent,
        ::std::string name) {
        auto folder_res = file.folders.resolve(parent);

        if (!folder_res) {
            return ::std::unexpected(folder_res.error());
        }

        core::LibraryFolder* folder_p = folder_res.value();

        core::FolderId new_folder = file.folders.add(name, parent);

        auto add_res = folder_p->add_folder(name, new_folder);

        if (!add_res) {
            return ::std::unexpected(add_res.error());
        }

        return new_folder;
    }

    ::std::expected<core::SymbolId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_symbol(
        core::AnimFile& file,
        core::FolderId parent,
        ::std::string name) {
        auto folder_res = file.folders.resolve(parent);

        if (!folder_res) {
            return ::std::unexpected(folder_res.error());
        }

        core::LibraryFolder* folder_p = folder_res.value();

        core::SymbolId new_symbol = file.symbols.add(name, parent);

        auto add_res = folder_p->add_symbol(name, new_symbol);

        if (!add_res) {
            return ::std::unexpected(add_res.error());
        }

        return new_symbol;
    }

    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename_symbol(
        core::AnimFile& file,
        core::SymbolId symbol,
        ::std::string new_name) {
        auto symbol_res = file.symbols.resolve(symbol);

        if (!symbol_res) {
            return ::std::unexpected(symbol_res.error());
        }

        core::Symbol* symbol_p = *symbol_res;

        auto parent_res = file.folders.resolve(symbol_p->parent());

        if (!parent_res) {
            ::qtil::panic("Symbol has invalid parent!");
        }

        core::LibraryFolder* parent_p = parent_res.value();

        auto rename_res = parent_p->rename(symbol_p->name(), new_name);

        if (!rename_res) {
            return ::std::unexpected(rename_res.error());
        }

        symbol_p->set_name(new_name);
    }

    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename_folder(
        core::AnimFile& file,
        core::FolderId folder,
        ::std::string new_name) {
        auto folder_res = file.folders.resolve(folder);

        if (!folder_res) {
            return ::std::unexpected(folder_res.error());
        }

        core::LibraryFolder* folder_p = folder_res.value();

        auto rename_res = folder_p->rename(folder_p->name(), new_name);

        if (!rename_res) {
            return ::std::unexpected(rename_res.error());
        }

        folder_p->set_name(new_name);
    }
}
