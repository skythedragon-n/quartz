//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Symbol.hpp"

namespace quartz::renderer {
    Symbol::Symbol(std::string name, Type type) :
    name_(name),
    type_(type) {}

    void Symbol::set_name(std::string name) {
        name_ = name;
    }
}
