//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "Symbol.hpp"

namespace quartz::renderer {
    class LibraryFolder {
        ::std::vector<Symbol> symbols_;
        ::std::vector<LibraryFolder> folders_;

        ::std::string name_;

    public:

        explicit LibraryFolder(::std::string name);

        void set_name(::std::string name);

        void add_symbol(::std::string name, Symbol::Type type);
        void add_folder(::std::string name);

        Symbol& find_symbol(std::string path);
        LibraryFolder& find_folder(std::string path);

        [[nodiscard]] ::std::string name() const { return name_; }
    };
}
