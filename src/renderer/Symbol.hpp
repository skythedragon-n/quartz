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
    class LibraryFolder;
    class Library;

    class Symbol {
    public:
        enum class Type {
            drawing
        };
    private:
        ::std::string name_;
        Type type_;
        ::std::variant<Drawing/**, std::vector<AnimatedLayer>**/> data_;

        LibraryFolder* parent_;
        void set_parent(LibraryFolder* parent);

        struct CtorKey {
        private:
            CtorKey() = default;
            friend class AnimFile;
        };

        friend class Library;
        friend class AnimFile;

    public:

        Symbol(CtorKey, ::std::string name, Type type, LibraryFolder* parent);

        void set_name(::std::string name);

        [[nodiscard]] ::std::string name() const { return name_; }

        Drawing& drawing() { return ::std::get<Drawing>(data_); }
    };
}
