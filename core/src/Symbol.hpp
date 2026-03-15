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
#include "AnimatedLayer.hpp"

namespace quartz::core {
    namespace symbol_types {
        struct LayeredAnimation {
            ::std::vector<AnimLayerId> layers;
        };

        struct Scene {
            ::std::vector<AnimatedLayer> layers;
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
        ::std::string name_;
        ::std::variant<symbol_types::LayeredAnimation, symbol_types::Scene, symbol_types::DrawingSymbol, symbol_types::Void> data_;

        AnimFile* file_ = nullptr;

        const SymbolId id_;

        FolderId parent_;

        friend class Library;
        friend class AnimFile;

    public:

        Symbol(IdKey, AnimFile* file, ::std::string name, FolderId parent, SymbolId id);

        void set_name(const ::std::string& name);

        void set_parent(FolderId parent);

        [[nodiscard]] ::std::string name() const { return name_; }

        [[nodiscard]] FolderId parent() const { return parent_; }

        template<symbol_types::SymbolType T>
        [[nodiscard]] bool is() const {
            return ::std::holds_alternative<T>(data_);
        }

        template<symbol_types::SymbolType T>
        [[nodiscard]] ::std::optional<T*> get() const {
            if (!::std::holds_alternative<T>(data_)) {
                return ::std::nullopt;
            }
            return &::std::get<T>(data_);
        }

        template<symbol_types::SymbolType T>
        void set(const T& data) {
            data_ = data;
        }

        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) {
            return ::std::visit(::std::forward<Visitor>(visitor), data_);
        }

        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) const {
            return std::visit(::std::forward<Visitor>(visitor), data_);
        }
    };
}
