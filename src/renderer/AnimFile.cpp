//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "AnimFile.hpp"

namespace quartz::renderer {
    AnimFile::AnimFile() {
    }

    void AnimFile::set_width(uint64_t width) {
        width_ = width;
    }

    void AnimFile::set_height(uint64_t height) {
        height_ = height;
    }

    void AnimFile::set_size(uint64_t width, uint64_t height) {
        width_ = width;
        height_ = height;
    }

    void AnimFile::set_fps(uint64_t fps) {
        fps_ = fps;
    }

    void AnimFile::add_library(::std::string group) {
        libraries_.emplace_back(Library::CtorKey(), group, this);
        libraries_by_group_[group] = libraries_.size() - 1;
    }

    SymbolId AnimFile::add_symbol(::std::string name, Symbol::Type type, LibraryFolder* parent) {
        symbols_.emplace_back(Symbol::CtorKey(), name, type, parent);
        return symbols_.size() - 1;
    }

    FolderId AnimFile::add_folder(::std::string name, LibraryFolder* parent) {
        folders_.emplace_back(LibraryFolder::CtorKey(), name, parent, this);
        return folders_.size() - 1;
    }

    FindResult<SymbolId> AnimFile::find_symbol(std::string path) {
        size_t colon = path.find_first_of(':');

        if (colon == ::std::string::npos) {
            return ::std::nullopt;
        }

        auto library_iter = libraries_by_group_.find(path.substr(0, colon));

        if (library_iter == libraries_by_group_.end()) {
            return ::std::nullopt;
        }

        return libraries_[library_iter->second].find_symbol(path.substr(colon + 1));
    }

    FindResult<FolderId> AnimFile::find_folder(std::string path) {
        size_t colon = path.find_first_of(':');

        if (colon == ::std::string::npos) {
            return ::std::nullopt;
        }

        auto library_iter = libraries_by_group_.find(path.substr(0, colon));

        if (library_iter == libraries_by_group_.end()) {
            return ::std::nullopt;
        }

        return libraries_[library_iter->second].find_folder(path.substr(colon + 1));
    }

    FindResult<LibraryId> AnimFile::get_library(std::string group) {
        if (libraries_by_group_.find(group) == libraries_by_group_.end()) {
            return LIBRARY_ID_INVALID;
        }

        return libraries_by_group_.at(group);
    }
}
