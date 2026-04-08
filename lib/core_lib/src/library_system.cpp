//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Feb 2026.
//

#include "./library_system.hpp"

#include <quartz/core/AnimFile.hpp>
#include <quartz/core/LibraryFolder.hpp>

#include <qtil/panic.hpp>
#include <qtil/overloads.hpp>

namespace quartz::lib::core_lib {
    ::std::expected<core::SymbolId, core::FindFailure> find_symbol_in_folder(
        core::AnimFile& file,
        core::FolderId folder_id,
        const ::std::string& path)
    {
        auto res = file.folders.resolve(folder_id);

        if (!res.has_value()) {
            ::qtil::panic("Folder contains invalid ID!");
        }

        core::LibraryFolder* folder_p = *res;

        size_t first_sep = path.find_first_of('/');

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

    ::std::expected<core::FolderId, core::FindFailure> find_folder_in_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& path)
    {
        auto res = file.folders.resolve(folder);

        if (!res.has_value()) {
            ::qtil::panic("Folder contains invalid ID!");
        }

        core::LibraryFolder* folder_p = *res;

        size_t first_sep = path.find_first_of('/');

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

        core::LibraryFolder* folder_p = *folder_res;

        if (!folder_p->available(name)) {
            return ::std::unexpected(core::AddFailure::NameInUse);
        }

        core::FolderId new_folder = file.folders.add(name, parent);

        auto add_res = folder_p->add_folder(name, new_folder);

        if (!add_res) {
            file.folders.free(new_folder);
            return ::std::unexpected(add_res.error());
        }

        return new_folder;
    }

    ::std::expected<core::FolderId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_folder(
        core::AnimFile& file,
        core::LibraryId parent,
        ::std::string name) {
        auto library_res = file.libraries.resolve(parent);

        if (!library_res) {
            return ::std::unexpected(library_res.error());
        }

        core::Library* library_p = *library_res;

        if (!library_p->available(name)) {
            return ::std::unexpected(core::AddFailure::NameInUse);
        }

        core::FolderId new_folder = file.folders.add(name, parent);

        auto add_res = library_p->add_folder(name, new_folder);

        if (!add_res) {
            file.folders.free(new_folder);
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

        core::LibraryFolder* folder_p = *folder_res;

        if (!folder_p->available(name)) {
            return ::std::unexpected(core::AddFailure::NameInUse);
        }

        core::SymbolId new_symbol = file.symbols.add(name, parent);

        auto add_res = folder_p->add_symbol(name, new_symbol);

        if (!add_res) {
            file.symbols.free(new_symbol);
            return ::std::unexpected(add_res.error());
        }

        return new_symbol;
    }

    ::std::expected<core::SymbolId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_symbol(
        core::AnimFile& file,
        core::LibraryId parent,
        ::std::string name) {
        auto library_res = file.libraries.resolve(parent);

        if (!library_res) {
            return ::std::unexpected(library_res.error());
        }

        core::Library* library_p = *library_res;

        if (!library_p->available(name)) {
            return ::std::unexpected(core::AddFailure::NameInUse);
        }

        core::SymbolId new_symbol = file.symbols.add(name, parent);

        auto add_res = library_p->add_symbol(name, new_symbol);

        if (!add_res) {
            file.symbols.free(new_symbol);
            return ::std::unexpected(add_res.error());
        }

        return new_symbol;
    }

    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename(
        core::AnimFile& file,
        core::SymbolId symbol,
        ::std::string new_name) {
        auto symbol_res = file.symbols.resolve(symbol);

        if (!symbol_res) {
            return ::std::unexpected(symbol_res.error());
        }

        core::Symbol* symbol_p = *symbol_res;

        auto parent_id = symbol_p->parent();

        using f_res = ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>>;

        return ::qtil::match(parent_id,
            [&file, symbol_p, &new_name](core::LibraryId library_id) -> f_res {
                auto parent_res = file.libraries.resolve(library_id);

                if (!parent_res) {
                    ::qtil::panic("Symbol has invalid parent!");
                }

                core::Library* parent_p = *parent_res;

                auto rename_res = parent_p->rename(symbol_p->name(), new_name);

                if (!rename_res) {
                    return ::std::unexpected(rename_res.error());
                }

                symbol_p->set_name(new_name);

                return {};
            },
            [&file, symbol_p, &new_name](core::FolderId folder_id) -> f_res {
                auto parent_res = file.folders.resolve(folder_id);

                if (!parent_res) {
                    ::qtil::panic("Symbol has invalid parent!");
                }

                core::LibraryFolder* parent_p = *parent_res;

                auto rename_res = parent_p->rename(symbol_p->name(), new_name);

                if (!rename_res) {
                    return ::std::unexpected(rename_res.error());
                }

                symbol_p->set_name(new_name);

                return {};
            });
    }

    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& new_name) {
        auto folder_res = file.folders.resolve(folder);

        if (!folder_res) {
            return ::std::unexpected(folder_res.error());
        }

        core::LibraryFolder* folder_p = *folder_res;

        auto parent_id = folder_p->parent();

        using f_res = ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>>;

        return ::qtil::match(parent_id,
            [&file, folder_p, &new_name](core::LibraryId library_id) -> f_res {
                auto parent_res = file.libraries.resolve(library_id);

                if (!parent_res) {
                    ::qtil::panic("Folder has invalid parent!");
                }

                core::Library* parent_p = *parent_res;

                auto rename_res = parent_p->rename(folder_p->name(), new_name);

                if (!rename_res) {
                    return ::std::unexpected(rename_res.error());
                }

                folder_p->set_name(new_name);

                return {};
            },
            [&file, folder_p, &new_name](core::FolderId folder_id) -> f_res {
                auto parent_res = file.folders.resolve(folder_id);

                if (!parent_res) {
                    ::qtil::panic("Folder has invalid parent!");
                }

                core::LibraryFolder* parent_p = *parent_res;

                auto rename_res = parent_p->rename(folder_p->name(), new_name);

                if (!rename_res) {
                    return ::std::unexpected(rename_res.error());
                }

                folder_p->set_name(new_name);

                return {};
            });
    }

    ::std::expected<core::SymbolId, core::FindFailure> find_symbol(
        core::AnimFile& file,
        ::std::string path) {
        size_t first_sep = path.find_first_of(':');

        if (first_sep == ::std::string::npos) {
            return ::std::unexpected(core::FindFailure::NoSuchPath);
        }

        auto library_res = file.get_library(path.substr(0, first_sep));

        if (!library_res) {
            return ::std::unexpected(library_res.error());
        }

        core::Library* library = *file.libraries.resolve(*library_res);

        ::std::string subpath = path.substr(first_sep + 1);

        size_t second_sep = subpath.find_first_of('/');

        if (second_sep == ::std::string::npos) {
            auto out_res = library->find_symbol(subpath.substr(0, second_sep));

            if (!out_res) {
                return ::std::unexpected(core::FindFailure::NoSuchPath);
            }

            return *out_res;
        }

        auto subfolder_res = library->find_folder(subpath.substr(0, second_sep));

        if (!subfolder_res) {
            return ::std::unexpected(core::FindFailure::NoSuchPath);
        }

        return find_symbol_in_folder(file, *subfolder_res, subpath.substr(second_sep + 1));
    }
}
