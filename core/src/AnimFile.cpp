//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "AnimFile.hpp"

namespace quartz::core {
    AnimFile::AnimFile() :
        symbols(this),
        folders(this),
        libraries(this),
        layers(this),
        frames(this)
        {}

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
        LibraryId id = libraries.add(group);
        libraries_by_group_[group] = id;
    }

    ::std::expected<LibraryId, FindFailure> AnimFile::get_library(::std::string group) {
        if (libraries_by_group_.find(group) == libraries_by_group_.end()) {
            return ::std::unexpected(FindFailure::NoSuchPath);
        }

        return libraries_by_group_.at(group);
    }
}
