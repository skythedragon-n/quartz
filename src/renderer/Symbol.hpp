//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <string>
#include <variant>

#include "Drawing.hpp"


namespace quartz::renderer {
    class Symbol {
    public:
        enum class Type {
            drawing
        };
    private:
        ::std::string name_;
        Type type_;
        ::std::variant<Drawing> data_;

    public:
        Symbol(::std::string name, Type type);

        void set_name(::std::string name);

        [[nodiscard]] ::std::string name() const { return name_; }

        Drawing& drawing() { return ::std::get<Drawing>(data_); }
    };
}
