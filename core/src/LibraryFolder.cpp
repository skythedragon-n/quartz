//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "LibraryFolder.hpp"

#include <utility>
#include <iostream>

#include "AnimFile.hpp"
#include "overloads.hpp"

namespace quartz::core {
    void LibraryFolder::set_parent(LibraryId parent) {
        parent_ = parent;
    }

    void LibraryFolder::set_parent(FolderId parent) {
        parent_ = parent;
    }

    LibraryFolder::LibraryFolder(AnimKey, ::std::string name, FolderId parent, FolderId id) :
    name_(std::move(name)),
    parent_(parent),
    id_(id)
    {}

    LibraryFolder::LibraryFolder(LibraryId parent) :
    name_("root"),
    parent_(parent),
    id_(FOLDER_ID_INVALID)
    {}

    void LibraryFolder::set_name(::std::string name) {
        name_ = std::move(name);
    }

    ::std::expected<SymbolId, FindFailure> LibraryFolder::find_symbol_by_path(::std::string path) {
        ::std::size_t first_sep = path.find_first_of('/');

        if (first_sep == ::std::string::npos) {
            auto iter = symbols_.find(path);

            if (iter == symbols_.end()) {
                return ::std::unexpected(FindFailure::NoSuchPath);
            }

            return iter->second;
        }

        auto iter = folders_.find(path.substr(0, first_sep));

        if (iter == folders_.end()) {
            return ::std::unexpected(FindFailure::NoSuchPath);
        }

        FolderId folder = iter->second;

        return folder.file->resolve_folder(folder).value()->find_symbol_by_path(path.substr(first_sep + 1));
    }

    ::std::expected<FolderId, FindFailure> LibraryFolder::find_folder_by_path(::std::string path) {
        ::std::size_t first_sep = path.find_first_of('/');

        if (first_sep == ::std::string::npos) {
            auto iter = folders_.find(path);

            if (iter == folders_.end()) {
                return ::std::unexpected(FindFailure::NoSuchPath);
            }

            return iter->second;
        }

        auto iter = folders_.find(path.substr(0, first_sep));

        if (iter == folders_.end()) {
            return ::std::unexpected(FindFailure::NoSuchPath);
        }

        FolderId folder = iter->second;

        return folder.file->resolve_folder(folder).value()->find_folder_by_path(path.substr(first_sep + 1));
    }

    ::std::expected<void, AddFailure> LibraryFolder::add_symbol(const ::std::string& name, SymbolId id) {
        if (symbols_.contains(name) || folders_.contains(name)) {
            return ::std::unexpected(AddFailure::NameInUse);
        }
        
        symbols_[name] = id;
        
        return {};
    }

    std::optional<SymbolId> LibraryFolder::remove_symbol(const std::string& name) {
        if (!symbols_.contains(name)) {
            return ::std::nullopt;
        }
        
        SymbolId id = symbols_.at(name);
        
        symbols_.erase(name);
        
        return id;
    }

    std::optional<SymbolId> LibraryFolder::find_symbol(const std::string& name) const {
        if (!symbols_.contains(name)) {
            return ::std::nullopt;
        }
        
        return symbols_.at(name);
    }

    std::expected<void, AddFailure> LibraryFolder::add_folder(const std::string& name, FolderId id) {
        if (symbols_.contains(name) || folders_.contains(name)) {
            return ::std::unexpected(AddFailure::NameInUse);
        }

        folders_[name] = id;

        return {};
    }

    std::optional<FolderId> LibraryFolder::remove_folder(const std::string& name) {
        if (!folders_.contains(name)) {
            return ::std::nullopt;
        }

        FolderId id = folders_.at(name);

        folders_.erase(name);

        return id;
    }

    std::optional<FolderId> LibraryFolder::find_folder(const std::string& name) const {
        if (!folders_.contains(name)) {
            return ::std::nullopt;
        }

        return folders_.at(name);
    }

    ::std::optional<::std::variant<SymbolId, FolderId>> LibraryFolder::find(const ::std::string& name) const {
        if (symbols_.contains(name)) {
            return symbols_.at(name);
        }

        if (folders_.contains(name)) {
            return folders_.at(name);
        }

        return ::std::nullopt;
    }

    ::std::optional<::std::variant<SymbolId, FolderId>> LibraryFolder::remove(const ::std::string& name) {
        if (symbols_.contains(name)) {
            SymbolId id = symbols_.at(name);

            symbols_.erase(name);

            return id;
        }

        if (folders_.contains(name)) {
            FolderId id = folders_.at(name);

            folders_.erase(name);

            return id;
        }

        return ::std::nullopt;
    }

    std::expected<void, RenameFailure> LibraryFolder::rename(const std::string& old_name, const std::string& new_name) {
        if (symbols_.contains(new_name) || folders_.contains(new_name)) {
            return ::std::unexpected(RenameFailure::NameInUse);
        }

        auto removed = remove(old_name);

        if (!removed) {
            return ::std::unexpected(RenameFailure::SourceNonexistant);
        }

        ::qtil::match(*removed,
            [&](SymbolId id) {
                if (!add_symbol(new_name, id)) {
                    ::std::cerr << "LIBRARYFOLDER MOVE FAILURE. REPORT IMMEDIATELY";
                    abort();
                }
            },
            [&](FolderId id) {
                if (!add_folder(new_name, id)) {
                    ::std::cerr << "LIBRARYFOLDER MOVE FAILURE. REPORT IMMEDIATELY";
                    abort();
                }
            });

        return {};
    }
}
