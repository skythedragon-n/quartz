//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <string>

#include "LibraryFolder.hpp"
#include "Symbol.hpp"
#include "core_errors.hpp"

namespace quartz::core {
    class Library {
        LibraryFolder root_;

        ::std::string group_;

        AnimFile* file_ = nullptr;

        friend class AnimFile;
        friend class LibraryFolder;

    public:

        Library(IdKey, AnimFile* file, ::std::string group, LibraryId id);

        /**
         * @brief Set Library group
         * @param group New group for library
         */
        void set_group(const std::string& group);

        /**
         * @brief Adds Symbol to Library
         * @param name Name of symbol to be added
         * @param id The id of Symbol to be added
         * @return Nothing or error
         */
        ::std::expected<void, AddFailure> add_symbol(const ::std::string& name, SymbolId id);
        /**
         * @brief Removes Symbol from Library
         * @param name Name of Symbol to attempt to remove
         * @return Id of the removed Symbol, or nothing if name is not a Symbol
         */
        ::std::optional<SymbolId> remove_symbol(const ::std::string& name);
        /**
         * @brief Finds symbol in Library
         * @param name Name of Symbol to attempt to find
         * @return Symbol's Id, or nothing if name is not a Symbol
         */
        [[nodiscard]] ::std::optional<SymbolId> find_symbol(const ::std::string& name) const;

        /**
         * @brief Add LibraryFolder to Library
         * @param name Name of LibraryFolder to be added
         * @param id LibraryFolder's Id
         * @return Nothing or error
         */
        ::std::expected<void, AddFailure> add_folder(const ::std::string& name, FolderId id);
        /**
         * @brief Attempts to remove LibraryFolder from Library
         * @param name Name of LibraryFolder to attempt to remove
         * @return Id of the removed LibraryFolder, or nothing if name is not a folder
         */
        ::std::optional<FolderId> remove_folder(const ::std::string& name);
        /**
         * @brief Finds folder in library
         * @param name Name of sub-LibraryFolder attempt to find
         * @return Folder's Id, or nothing if name is not a folder
         */
        [[nodiscard]] ::std::optional<FolderId> find_folder(const ::std::string& name) const;

        /**
         * @brief Finds an object in Library
         * @param name Name of object to find
         * @return Id of found thing, folder or Symbol. If the name does not exist, it returns nothing
         */
        [[nodiscard]] ::std::optional<::std::variant<SymbolId, FolderId>> find(const ::std::string& name) const;
        /**
         * @brief Attempts to remove object from Library
         * @param name Name of object to remove
         * @return Id of the removed object, or nothing if name does not exist
         */
        ::std::optional<::std::variant<SymbolId, FolderId>> remove(const ::std::string& name);

        /**
         * @brief Attempts to rename an object in this library
         * @param old_name Name of target
         * @param new_name Target's new name
         * @return Nothing, or error if name does not exist, or target already exists
         */
        ::std::expected<void, RenameFailure> rename(const ::std::string& old_name, const ::std::string& new_name);

        [[nodiscard]] bool available(const ::std::string& name) const;

        /**
         * @brief Get Library's group
         * @return Library's group
         */
        [[nodiscard]] ::std::string group() const { return group_; }

        auto folders() { return root_.folders(); }

        auto symbols() { return root_.symbols(); }
    };
}
