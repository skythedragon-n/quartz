//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include "./Symbol.hpp"

#include <utility>

namespace quartz::core {
    Symbol::Symbol(IdKey, AnimFile* file, ::std::string name, FolderId parent, SymbolId id) :
        name_(std::move(name)),
        data_(symbol_types::Void{}),
        file_(file),
        id_(id),
        parent_(parent)
    {}

    Symbol::Symbol(IdKey, AnimFile* file, ::std::string name, LibraryId parent, SymbolId id) :
        name_(std::move(name)),
        data_(symbol_types::Void{}),
        file_(file),
        id_(id),
        parent_(parent)
    {}

    Symbol::Symbol(IdKey, AnimFile* file, const Symbol& symbol, SymbolId id) :
        name_(symbol.name()),
        data_(symbol.data()),
        file_(file),
        id_(id),
        parent_(symbol.parent())
    {}

    void Symbol::set_name(const ::std::string& name) {
        name_ = name;
    }

    void Symbol::set_parent(const FolderId parent) {
        parent_ = parent;
    }
}
