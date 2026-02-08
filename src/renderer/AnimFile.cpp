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
        //TODO: implement library
    }
}
