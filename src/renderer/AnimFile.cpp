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
        libraries_.emplace_back(
            Library::CtorKey(),
            group,
            LibraryId {libraries_.size(), this},
            this
        );
        libraries_by_group_[group] = {libraries_.size() - 1, this};
    }

    SymbolId AnimFile::add_symbol(::std::string name, Symbol::Type type, FolderId parent) {
        symbols_.emplace_back(
            Symbol::CtorKey(),
            name,
            type,
            parent,
            SymbolId {symbols_.size(), this},
            this
        );
        return {symbols_.size() - 1, this};
    }

    FolderId AnimFile::add_folder(::std::string name, FolderId parent) {
        folders_.emplace_back(
            LibraryFolder::CtorKey(),
            name,
            parent,
            FolderId {folders_.size(), this},
            this
        );
        return {folders_.size() - 1, this};
    }

    FindResult<SymbolId> AnimFile::find_symbol(::std::string path) {
        size_t colon = path.find_first_of(':');

        if (colon == ::std::string::npos) {
            return ::std::nullopt;
        }

        auto library_iter = libraries_by_group_.find(path.substr(0, colon));

        if (library_iter == libraries_by_group_.end()) {
            return ::std::nullopt;
        }

        return libraries_[library_iter->second.id].find_symbol(path.substr(colon + 1));
    }

    FindResult<FolderId> AnimFile::find_folder(::std::string path) {
        size_t colon = path.find_first_of(':');

        if (colon == ::std::string::npos) {
            return ::std::nullopt;
        }

        auto library_iter = libraries_by_group_.find(path.substr(0, colon));

        if (library_iter == libraries_by_group_.end()) {
            return ::std::nullopt;
        }

        return resolve(library_iter->second).value()->find_folder(path.substr(colon + 1));
    }

    FindResult<LibraryId> AnimFile::get_library(::std::string group) {
        if (libraries_by_group_.find(group) == libraries_by_group_.end()) {
            return LIBRARY_ID_INVALID;
        }

        return libraries_by_group_.at(group);
    }

    ::std::expected<Symbol*, ResolveFailure> AnimFile::resolve_symbol(SymbolId id) {
        if (id == SYMBOL_ID_INVALID) {
            return ::std::unexpected(ResolveFailure::InvalidId);
        }

        if (id.file != this) {
            return ::std::unexpected(ResolveFailure::WrongFile);
        }

        return &symbols_[id.id];
    }

    ::std::expected<LibraryFolder*, ResolveFailure> AnimFile::resolve_folder(FolderId id) {
        if (id == FOLDER_ID_INVALID) {
            return ::std::unexpected(ResolveFailure::InvalidId);
        }

        if (id.file != this) {
            return ::std::unexpected(ResolveFailure::WrongFile);
        }

        return &folders_[id.id];
    }

    ::std::expected<Library*, ResolveFailure> AnimFile::resolve_library(LibraryId id) {
        if (id == LIBRARY_ID_INVALID) {
            return ::std::unexpected(ResolveFailure::InvalidId);
        }

        if (id.file != this) {
            return ::std::unexpected(ResolveFailure::WrongFile);
        }

        return &libraries_[id.id];
    }
}
