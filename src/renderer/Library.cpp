//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Library.hpp"

namespace quartz::renderer {
    Library::Library(std::string group) : group_(group), root_("root") {
    }

    void Library::add_folder(std::string name) {
        root_.add_folder(name);
    }

    void Library::add_symbol(std::string name, Symbol::Type type) {
        root_.add_symbol(name, type);
    }

    Symbol& Library::find_symbol(std::string path) {
        return root_.find_symbol(path);
    }

    LibraryFolder& Library::find_folder(std::string path) {
        return root_.find_folder(path);
    }
}
