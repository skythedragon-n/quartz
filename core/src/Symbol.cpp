//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Symbol.hpp"

#include <utility>

namespace quartz::core {
    Symbol::Symbol(IdKey, AnimFile* file, ::std::string name, FolderId parent, SymbolId id) :
    name_(std::move(name)),
    file_(file),
    id_(id),
    parent_(parent)
    {}

    void Symbol::set_name(const ::std::string& name) {
        name_ = name;
    }

    void Symbol::set_parent(const FolderId parent) {
        parent_ = parent;
    }
}
