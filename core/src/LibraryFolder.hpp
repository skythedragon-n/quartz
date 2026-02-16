//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "libraryutils.hpp"

namespace quartz::core {
    /**
     * @brief Folder containing symbols, or other folders
     *
     * @authors SkyTheDragon
     *
     * A folder which can contain symbols, or other folders, which tracks the parent folder/library.
     */
    class LibraryFolder {
        ::std::unordered_map<::std::string, SymbolId> symbols_;
        ::std::unordered_map<::std::string, FolderId> folders_;

        ::std::string name_;

        std::variant<LibraryId, FolderId> parent_;

        AnimFile* file_ = nullptr;

        const FolderId id_;

        /**
         * @brief Sets LibraryFolder's parent to the inputted library
         * @param parent Library to make new parent
         */
        void set_parent(LibraryId parent);

        /**
         * @brief Set LibraryFolder's parent to the inputted folder
         * @param parent LibraryFolder to make new parent
         */
        void set_parent(FolderId parent);

        struct CtorKey {
        private:
            CtorKey() = default;
            friend class AnimFile;
        };

        explicit LibraryFolder(LibraryId parent, AnimFile* file);

        friend class Library;
        friend class AnimFile;

    public:

        LibraryFolder(CtorKey, ::std::string name, FolderId parent, FolderId id, AnimFile* file);

        /**
         * @brief Set folder name to inputted name
         * @param name New name for folder
         */
        void set_name(::std::string name);

        /**
         * @brief Add a symbol to the folder
         * @param name Name of symbol to be added
         * @param type Type of symbol
         */
        [[nodiscard]] ::std::expected<void, > add_symbol(::std::string name, Symbol::Type type);
        void add_folder(::std::string name);

        ::std::expected<SymbolId, FindFailure> find_symbol(std::string path);
        ::std::expected<FolderId, FindFailure> find_folder(std::string path);

        ::std::expected<void, FindFailure> rename(::std::string old_name, ::std::string new_name);

        /**
         * @brief Gets the name of this folder
         * @return Name of the folder
         */
        [[nodiscard]] ::std::string name() const { return name_; }
    };
}
