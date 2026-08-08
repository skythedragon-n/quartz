//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
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
            ::std::vector<AnimLayerId> layers;
        };

        struct DrawingSymbol {
            DrawingId drawing;
        };

        struct Void {};

        template<typename T>
        concept SymbolType =
            ::std::same_as<T, LayeredAnimation>
        ||  ::std::same_as<T, Scene>
        ||  ::std::same_as<T, DrawingSymbol>
        ||  ::std::same_as<T, Void>;
    }

    /**
     * @class Symbol
     * @brief A symbol in the library tree. These are the leaf nodes, and contain all of the actual animation data.
     *
     * @authors SkyTheDragon
     */
    class Symbol {
        using SymbolData = ::std::variant<
            symbol_types::LayeredAnimation,
            symbol_types::Scene,
            symbol_types::DrawingSymbol,
            symbol_types::Void>;
        ::std::string name_;
        SymbolData data_;

        AnimFile* file_;

        const SymbolId id_;

        ::std::variant<FolderId, LibraryId> parent_;

        friend class Library;
        friend class AnimFile;

    public:

        Symbol(IdKey, AnimFile* file, ::std::string name, FolderId parent, SymbolId id);
        Symbol(IdKey, AnimFile* file, ::std::string name, LibraryId parent, SymbolId id);
        Symbol(IdKey, AnimFile* file, const Symbol& symbol, SymbolId id);

        /**
         * @brief Gets an immutable reference to the symbol's data. Why would you need this?
         * @deprecated Probably going to be removed by 0.1.0. I don't know why I have this method anymore, I just don't
         *  feel like deleting it RN. I mean, the only real use-case is checking if the symbol data is equal to that of
         * another, but the rest is covered by my wonderful API. Now I will stop this comment before I get introspective.
         * @return Immutable reference to symbol data
         */
        [[nodiscard]] const SymbolData& data() const { return data_; }

        /**
         * @brief Sets symbol's name. Don't call this yourself, at near-gaurunteed breakage of the library tree.
         * Unless you're in the core ibrar management code. Then be careful.
         * @param name Name to be set
         */
        void set_name(const ::std::string& name);

        /**
         * @brief Sets symbol's parent. Don't call this method yourself, unless you are in the core library management code
         * @param parent Value to be set
         */
        void set_parent(FolderId parent);

        /**
         * @brief Gets the name of symbol.
         * @return Name of symbol
         */
        [[nodiscard]] ::std::string name() const { return name_; }

        /**
         * @brief Gets the parent library node to this symbol
         * @return Parent of symbol
         */
        [[nodiscard]] ::std::variant<FolderId, LibraryId> parent() const { return parent_; }

        /**
         * @brief Checks if a symbol is of a certain type
         * @tparam T Symbol type to check for
         * @return Is the symbol of the specified type
         */
        template<symbol_types::SymbolType T>
        [[nodiscard]] bool is() const {
            return ::std::holds_alternative<T>(data_);
        }

        /**
         * @brief Returns a pointer to symbol data, but only if the type in the symbol matches the type requested
         * @tparam T Symbol type to be gotten
         * @return An optional pointer to the data. This project doesn't subscribe to the implicit nullability of
         * pointers, so I use an optional to make it more explicit
         */
        template<symbol_types::SymbolType T>
        [[nodiscard]] ::std::optional<T*> get() const {
            if (!::std::holds_alternative<T>(data_)) {
                return ::std::nullopt;
            }
            const T* t = ::std::addressof(::std::get<T>(data_));
            return const_cast<T*>(t);
        }

        /**
         * @brief Allows the user to set the symbol data and change its type
         * @tparam T Symbol type of data being set
         * @param data Data
         */
        template<symbol_types::SymbolType T>
        void set(const T& data) {
            data_ = data;
        }

        /**
         * @brief Allows matching a Symbol by its type
         * @tparam Visitor Visitor type to be used
         * @param visitor instance of visitor type
         * @return Result of visitor visiting the symbol
         */
        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) {
            return ::std::visit(::std::forward<Visitor>(visitor), data_);
        }

        /**
        * @brief Allows matching a Symbol by its type, but const
        * @tparam Visitor Visitor type to be used
        * @param visitor instance of visitor type
        * @return Result of visitor visiting the symbol
        */
        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) const {
            return std::visit(::std::forward<Visitor>(visitor), data_);
        }
    };
}
