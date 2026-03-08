//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <optional>
#include <string>
#include <variant>

#include "Drawing.hpp"
#include "id_sys.hpp"

namespace quartz::core {
    namespace symbol_types {
        struct LayeredAnimation {
            std::vector<AnimatedLayer> layers;
        };

        struct Scene {
            std::vector<AnimatedLayer> layers;
        };

        struct DrawingSymbol {
            Drawing drawing;
        };

        struct Void {};

        template<typename T>
        concept SymbolType =
            ::std::same_as<T, LayeredAnimation>
        ||  ::std::same_as<T, Scene>
        ||  ::std::same_as<T, DrawingSymbol>
        ||  ::std::same_as<T, Void>;
    }

    class Symbol {
    public:
        enum class Type {
            drawing
        };
    private:
        ::std::string name_;
        Type type_;
        ::std::variant<symbol_types::LayeredAnimation, symbol_types::Scene, symbol_types::DrawingSymbol, symbol_types::Void> data_;

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

        template<symbol_types::SymbolType T>
        [[nodiscard]] bool is() const {
            return std::holds_alternative<T>(data_);
        }

        template<symbol_types::SymbolType T>
        [[nodiscard]] ::std::optional<T&> get() {
            if (!std::holds_alternative<T>(data_)) {
                return ::std::nullopt;
            }
            return ::std::get<T>(data_);
        }

        template<symbol_types::SymbolType T>
        void set(T& data) {
            data_ = data;
        }
    };
}
