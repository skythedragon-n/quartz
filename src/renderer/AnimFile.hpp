//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "Library.hpp"


namespace quartz::renderer {
    class AnimFile {
        ::std::vector<Library> libraries_;
        uint64_t width_ = 0, height_ = 0;
        uint64_t fps_ = 0;

    public:
        AnimFile();

        void set_width(uint64_t width);
        void set_height(uint64_t height);
        void set_size(uint64_t width, uint64_t height);
        void set_fps(uint64_t fps);

        void add_library(std::string group);

        Symbol& find_symbol(std::string path);
        LibraryFolder& find_folder(std::string path);
        Library& get_library(std::string group);
    };
}
