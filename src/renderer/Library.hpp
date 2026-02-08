//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <string>

#include "LibraryFolder.hpp"


namespace quartz::renderer {
    class Library {
        LibraryFolder root_;

        ::std::string group_;
    public:
        Library(::std::string group);

        void add_symbol(::std::string name, Symbol::Type type);
        void add_folder(::std::string name);

        Symbol& find_symbol(::std::string path);
        LibraryFolder& find_folder(::std::string path);

        [[nodiscard]] ::std::string group() const { return group_; }
    };
}
