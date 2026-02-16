//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "LibraryFolder.hpp"

#include "AnimFile.hpp"
#include "Library.hpp"

namespace quartz::core {
    void LibraryFolder::set_parent(LibraryId parent) {
        parent_ = parent;
    }

    void LibraryFolder::set_parent(FolderId parent) {
        parent_ = parent;
    }

    LibraryFolder::LibraryFolder(CtorKey, ::std::string name, FolderId parent, FolderId id, AnimFile* file) :
    name_(name),
    parent_(parent),
    id_(id),
    file_(file)
    {}

    LibraryFolder::LibraryFolder(LibraryId parent, AnimFile* file) :
    name_("root"),
    parent_(parent),
    id_(FOLDER_ID_INVALID),
    file_(file)
    {}

    void LibraryFolder::set_name(::std::string name) {
        name_ = name;
    }

    void LibraryFolder::add_symbol(::std::string name, Symbol::Type type) {
        SymbolId symbol = file_->add_symbol(name, type, id_);
        symbols_.emplace(name, symbol);
    }

    void LibraryFolder::add_folder(::std::string name) {
        FolderId folder = file_->add_folder(name, id_);
        folders_.emplace(name, folder);
    }

    ::std::expected<SymbolId, FindFailure> LibraryFolder::find_symbol(::std::string path) {
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

        return resolve(iter->second).value()->find_symbol(path.substr(first_sep + 1));
    }

    ::std::expected<FolderId, FindFailure> LibraryFolder::find_folder(::std::string path) {
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

        return resolve(iter->second).value()->find_folder(path.substr(first_sep + 1));
    }
}
