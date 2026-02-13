//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Symbol.hpp"

namespace quartz::renderer {
    Symbol::Symbol(CtorKey, ::std::string name, Type type, FolderId parent, SymbolId id, AnimFile* file) :
    name_(name),
    type_(type),
    parent_(parent),
    id_(id),
    file_(file)
    {}

    void Symbol::set_name(::std::string name) {
        name_ = name;
    }

    void Symbol::set_parent(FolderId parent) {
        parent_ = parent;
    }
}
