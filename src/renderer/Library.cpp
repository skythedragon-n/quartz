//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Library.hpp"

namespace quartz::renderer {
    Library::Library(CtorKey, std::string group, AnimFile* file) :
    group_(group),
    root_(this),
    file_(file)
    {}

    void Library::add_folder(std::string name) {
        root_.add_folder(name);
    }

    void Library::add_symbol(std::string name, Symbol::Type type) {
        root_.add_symbol(name, type);
    }

    FindResult<SymbolId> Library::find_symbol(std::string path) {
        return root_.find_symbol(path);
    }

    FindResult<FolderId> Library::find_folder(std::string path) {
        return root_.find_folder(path);
    }
}
