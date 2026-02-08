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

    void AnimFile::add_library(std::string group) {
        libraries_.emplace_back(group);
    }

    Symbol& AnimFile::find_symbol(std::string path) {
        size_t colon = path.find_first_of(':');

        //TODO: hande errors & reject invalid paths

        return get_library(path.substr(0, colon)).find_symbol(path.substr(colon + 1));
    }

    LibraryFolder& AnimFile::find_folder(std::string path) {
        size_t colon = path.find_first_of(':');

        //TODO: hande errors & reject invalid paths

        return get_library(path.substr(0, colon)).find_folder(path.substr(colon + 1));
    }

    Library& AnimFile::get_library(std::string group) {
        //TODO: handle errors & reject invalid paths
        for (Library& library : libraries_) {
            if (library.group() == group) {
                return library;
            }
        }
    }
}
