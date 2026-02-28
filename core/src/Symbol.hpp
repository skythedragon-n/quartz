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
#include "id_sys.hpp"

namespace quartz::core {

    class Symbol {
    public:
        enum class Type {
            drawing
        };
    private:
        ::std::string name_;
        Type type_;
        ::std::variant<Drawing/**, std::vector<AnimatedLayer>**/> data_;

        AnimFile* file_ = nullptr;

        const SymbolId id_;

        FolderId parent_;
        void set_parent(FolderId parent);

        friend class Library;
        friend class AnimFile;

    public:

        Symbol(AnimKey, ::std::string name, Type type, FolderId parent, SymbolId id, AnimFile* file);

        void set_name(::std::string name);

        [[nodiscard]] ::std::string name() const { return name_; }

        Drawing& drawing() { return ::std::get<Drawing>(data_); }
        //::std::vector<AnimatedLayer>& anim_layers() { return ::std::get<std::vector<AnimatedLayer>>(data_); }
    };
}
