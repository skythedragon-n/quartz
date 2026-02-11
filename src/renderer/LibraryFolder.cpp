//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "LibraryFolder.hpp"

#include "AnimFile.hpp"
#include "Library.hpp"

namespace quartz::renderer {
    void LibraryFolder::set_parent(LibraryFolder* parent) {
        parent_ = parent;
    }

    LibraryFolder::LibraryFolder(CtorKey, ::std::string name, LibraryFolder* parent, AnimFile* file) :
    name_(name),
    parent_(parent)
    {}

    LibraryFolder::LibraryFolder(Library* parent) :
    name_("root"),
    parent_(parent),
    file_(parent->file_)
    {}

    void LibraryFolder::set_name(::std::string name) {
        name_ = name;
    }

    void LibraryFolder::add_symbol(::std::string name, Symbol::Type type) {
        SymbolId symbol = file_->add_symbol(name, type, this);
        symbols_.emplace(name, symbol);
    }

    void LibraryFolder::add_folder(::std::string name) {
        FolderId folder = file_->add_folder(name, this);
        folders_.emplace(name, folder);
    }

    FindResult<SymbolId> LibraryFolder::find_symbol(std::string path) {
        ::std::size_t first_sep = path.find_first_of('/');

        if (first_sep == ::std::string::npos) {
            auto iter = symbols_.find(path);

            if (iter == symbols_.end()) {
                return std::nullopt;
            }

            return iter->second;
        }

        auto iter = folders_.find(path.substr(0, first_sep));

        if (iter == folders_.end()) {
            return ::std::nullopt;
        }

        return file_->resolve_folder(iter->second).find_symbol(path.substr(first_sep + 1));
    }

    FindResult<FolderId> LibraryFolder::find_folder(std::string path) {
        ::std::size_t first_sep = path.find_first_of('/');

        if (first_sep == ::std::string::npos) {
            auto iter = folders_.find(path);

            if (iter == folders_.end()) {
                return std::nullopt;
            }

            return iter->second;
        }

        auto iter = folders_.find(path.substr(0, first_sep));

        if (iter == folders_.end()) {
            return ::std::nullopt;
        }

        return file_->resolve_folder(iter->second).find_folder(path.substr(first_sep + 1));
    }
}
