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

    void LibraryFolder::add_symbol(::std::string name, Symbol::Type type) {
        symbols_.emplace_back(name, type);
    }
    void LibraryFolder::add_folder(::std::string name) {
        folders_.emplace_back(name);
    }

    Symbol& LibraryFolder::find_symbol(std::string path) {

        size_t slash = path.find_first_of('/');

        if (slash != std::string::npos) {
            return find_folder(path.substr(0, slash)).find_symbol(path.substr(slash + 1));
        }

        for (Symbol& symbol : symbols_) {
            if (symbol.name() == path) {
                return symbol;
            }
        }
    }

    LibraryFolder& LibraryFolder::find_folder(std::string path) {

        size_t slash = path.find_first_of('/');

        ::std::string folder_name = path.substr(0, slash);

        if (folder_name.find_first_of('/') != std::string::npos) {
            return find_folder(folder_name).find_folder(path.substr(slash + 1));
        }

        for (LibraryFolder& folder : folders_) {
            if (folder.name() == path) {
                return folder;
            }
        }
    }
}
