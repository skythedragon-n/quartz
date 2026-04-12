//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Feb 2026.
//

#pragma once

#include <expected>
#include <string>
#include <quartz/core/core_errors.hpp>
#include <quartz/core/id_sys.hpp>
#include <quartz/core/AnimFile.hpp>

/**
 * @file library_system.hpp
 *
 * @authors SkyTheDragon
 *
 * Contains various functions for manipulating & querying the library.
 */
namespace quartz::lib::core_lib {
    /**
     * @brief Attempts to find a symbol at the inputted path within the folder
     * @param file File to perform operations on
     * @param folder Folder to find symbol in
     * @param path Path of symbol to be found
     * @return The Id of the found symbol, or an error
     */
    ::std::expected<core::SymbolId, core::FindFailure> find_symbol_in_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& path);

    /**
     * @brief Attempts to find a folder at the inputted path within the folder
     * @param file File to perform operations on
     * @param folder Folder to find folder in
     * @param path Path of folder to be found
     * @return The Id of the folder, or an error
     */
    ::std::expected<core::FolderId, core::FindFailure> find_folder_in_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& path);

    /**
     * @brief Attempts to add subfolder to folder
     * @param file File to perform operations on
     * @param parent The folder to attempt to add to
     * @param name The name to give the new folder
     * @return New folder's Id, or error
     */
    ::std::expected<core::FolderId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_folder(
        core::AnimFile& file,
        core::FolderId parent,
        ::std::string name);

    /**
     * @brief Attempts to add folder to library
     * @param file File to perform operations on
     * @param parent The library to attempt to add to
     * @param name The name to give the new folder
     * @return New folder's Id, or error
     */
    ::std::expected<core::FolderId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_folder(
        core::AnimFile& file,
        core::LibraryId parent,
        ::std::string name);

    /**
     * @brief Attempts to add symbol to folder
     * @param file File to perform operations on
     * @param parent The folder to attempt to add to
     * @param name The to name give to the new symbol
     * @return New symbol's Id, or error
     */
    ::std::expected<core::SymbolId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_symbol(
        core::AnimFile& file,
        core::FolderId parent,
        ::std::string name);

    /**
     * Attempts to add symbol to library
     * @param file File to perform operations on
     * @param parent The library to attempt to add to
     * @param name Name of the new symbol
     * @return New symbol's Id, or error
     */
    ::std::expected<core::SymbolId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_symbol(
        core::AnimFile& file,
        core::LibraryId parent,
        ::std::string name);

    /**
     * @brief Attempts to rename symbol
     * @param file File to perform operations on
     * @param symbol The symbol to attempt to rename
     * @param new_name New name of symbol
     * @return Nothing, or error
     */
    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename(
        core::AnimFile& file,
        core::SymbolId symbol,
        ::std::string new_name);

    /**
     * @brief Attempts to rename folder
     * @param file File to perform operartions on
     * @param folder The folder to attempt to rename
     * @param new_name New name of folder
     * @return Nothing, or error
     */
    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& new_name);

    /**
     * @brief Finds symbol at path
     * @param file File to perform operations on
     * @param path The path to find symbol at
     * @return Id of found symbol, error
     */
    ::std::expected<core::SymbolId, core::FindFailure> find_symbol(
        core::AnimFile& file,
        ::std::string path);

    /**
     * @brief Finds folder at path
     * @param file File to perform operations on
     * @param path The path to find folder at
     * @return Id of foud folder, or error
     */
    ::std::expected<core::FolderId, core::FindFailure> find_folder(
        core::AnimFile& file,
        ::std::string path);

    ::std::expected<::std::string, core::ResolveFailure> build_path(
        core::AnimFile& file,
        core::FolderId folder);

    ::std::expected<::std::string, core::ResolveFailure> build_path(
        core::AnimFile& file,
        core::SymbolId symbol);

    ::std::expected<::std::string, core::ResolveFailure> build_path(
        core::AnimFile& file,
        core::LibraryId library);
}
