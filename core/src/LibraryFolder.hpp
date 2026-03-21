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
#include <expected>

#include "id_sys.hpp"
#include "core_errors.hpp"
#include "Symbol.hpp"

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

        ::std::variant<LibraryId, FolderId> parent_;

        const FolderId id_;

        explicit LibraryFolder(LibraryId parent);

        friend class Library;
        friend class AnimFile;

    public:

        LibraryFolder(IdKey, AnimFile*, ::std::string name, FolderId parent, FolderId id);

        /**
         * @brief Set LibraryFolder's parent to the inputted folder
         * @param parent LibraryFolder to make new parent
         */
        void set_parent(::std::variant<LibraryId, FolderId> parent);

        /**
         * @brief Set folder name to inputted name
         * @param name New name for folder
         */
        void set_name(::std::string name);

        /**
         * @brief Add a symbol to the folder
         * @param name Name of symbol to be added
         * @deprecated LibrayFolder API reconfiguration removes this method
         */
        [[nodiscard]] ::std::expected<void, AddFailure> add_symbol(::std::string name);
        /**
         * @brief Adds a subfolder to the folder
         * @deprecated LibrayFolder API reconfiguration removes this method
         */
        [[nodiscard]] ::std::expected<void, AddFailure> add_folder(::std::string name);

        /**
         * @brief finds Symbol at path
         * @param path path of Symbol to find
         * @return Symbols's ID, or error
         * @deprecated LibrayFolder API reconfiguration removes this method
         */
        [[deprecated]] ::std::expected<SymbolId, FindFailure> find_symbol_by_path(std::string path);
        /**
         * @brief finds LibraryFolder at path
         * @param path path of LibraryFolder to find
         * @return LibraryFolder's ID, or error
         * @deprecated LibrayFolder API reconfiguration removes this method
         */
        [[deprecated]] ::std::expected<FolderId, FindFailure> find_folder_by_path(std::string path);

        /**
         * @brief Adds Symbol to folder
         * @param name Name of symbol to be added
         * @param id The id of Symbol to be added
         * @return Nothing or error
         */
        ::std::expected<void, AddFailure> add_symbol(const ::std::string& name, SymbolId id);
        /**
         * @brief Removes Symbol from folder
         * @param name Name of Symbol to attempt to remove
         * @return Id of the removed Symbol, or nothing if name is not a Symbol
         */
        ::std::optional<SymbolId> remove_symbol(const ::std::string& name);
        /**
         * @brief Finds symbol in folder
         * @param name Name of Symbol to attempt to find
         * @return Symbol's Id, or nothing if name is not a Symbol
         */
        [[nodiscard]] ::std::optional<SymbolId> find_symbol(const ::std::string& name) const;

        /**
         * @brief Add sub-LibraryFolder to folder
         * @param name Name of LibraryFolder to be added
         * @param id LibraryFolder's Id
         * @return Nothing or error
         */
        ::std::expected<void, AddFailure> add_folder(const ::std::string& name, FolderId id);
        /**
         * @brief Attempts to remove sub-LibraryFolder from folder
         * @param name Name of LibraryFolder to attempt to remove
         * @return Id of the removed LibraryFolder, or nothing if name is not a folder
         */
        ::std::optional<FolderId> remove_folder(const ::std::string& name);
        /**
         * @brief Finds folder in folder
         * @param name Name of sub-LibraryFolder attempt to find
         * @return Folder's Id, or nothing if name is not a folder
         */
        [[nodiscard]] ::std::optional<FolderId> find_folder(const ::std::string& name) const;

        /**
         * @brief Finds an object in folder
         * @param name Name of thing to find
         * @return Id of found thing, folder or Symbol. If the name does not exist, it returns nothing
         */
        [[nodiscard]] ::std::optional<::std::variant<SymbolId, FolderId>> find(const ::std::string& name) const;
        /**
         * @brief Attempts to remove object from folder
         * @param name Name of thing to remove
         * @return Id of the removed object, or nothing if name does not exist
         */
        ::std::optional<::std::variant<SymbolId, FolderId>> remove(const ::std::string& name);

        /**
         * @brief Attempts to rename an object in this folder
         * @param old_name Name of target
         * @param new_name Target's new name
         * @return Nothing, or error if name does not exist
         */
        ::std::expected<void, RenameFailure> rename(const ::std::string& old_name, const ::std::string& new_name);

        /**
         * @brief Checks if name is available.
         * @param name Name to check availability of
         * @return Is name available?
         */
        bool open(const ::std::string& name) const;

        /**
         * @brief Gets the name of this folder
         * @return Name of the folder
         */
        [[nodiscard]] ::std::string name() const { return name_; }

        /**
         * @brief Gets folder's parent
         * @return Folder's parent's Id
         */
        [[nodiscard]] ::std::variant<LibraryId, FolderId> parent() const { return parent_; }
    };
}
