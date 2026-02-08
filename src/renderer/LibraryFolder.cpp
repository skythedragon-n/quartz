//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "LibraryFolder.hpp"

namespace quartz::renderer {
    LibraryFolder::LibraryFolder(::std::string name) :
    name_(name)
    {}

    void LibraryFolder::set_name(::std::string name) {
        name_ = name;
    }

    void LibraryFolder::add_symbol(::std::string name, ::std::string type) {
        //TODO: implement symbols
    }
    void LibraryFolder::add_folder(::std::string name) {
        items_.emplace_back(LibraryFolder(name));
    }

    Symbol& LibraryFolder::find_symbol(std::string path) {
        //TODO: implement
    }

    LibraryFolder& LibraryFolder::find_folder(std::string path) {
        //TODO: implement
    }
}
