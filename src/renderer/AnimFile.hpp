//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <cstdint>
#include <expected>
#include <string>
#include <unordered_map>
#include <vector>

#include "Library.hpp"
#include "libraryutils.hpp"


namespace quartz::renderer {
    /**
     * @brief Root of internal representation of animations
     *
     * @authors SkyTheDragon
     *
     * AnimFile is the primary interface for managing the data of animations in Quartz Renderer.
     * It holds all objects in an animation, including Symbols, Instances & Libraries within vectors,
     * which are indexed by the object's Ids
     */
    class AnimFile {
        ::std::vector<Library> libraries_;
        ::std::vector<Symbol> symbols_;
        ::std::vector<LibraryFolder> folders_;

        ::std::unordered_map<::std::string, LibraryId> libraries_by_group_;

        uint64_t width_ = 0, height_ = 0;
        uint64_t fps_ = 0;

        /**
         * @brief Adds a new @link Symbol @endlink to the document
         * @param name name of the symbol to be added
         * @param type Type of the symbol to be added
         * @param parent The parent folder of the symbol
         * @return Returns the Id of the newly minted symbol
         *
         * Adds a new symbol the symbol index with the parameters inputted, and returns the Id of the symbol
         * that has just been created.
         */
        SymbolId add_symbol(::std::string name, Symbol::Type type, FolderId parent);
        /**
         * @brief Adds a new @link LibraryFolder @endlink to the document
         * @param name Name of the folder to be added
         * @param parent The parent folder of this folder
         * @return The Id of the newly minted folder
         *
         * Adds a new folder to the folder index with the parmeters inputted, and return the Id
         * of the newly created folder
         */
        FolderId add_folder(::std::string name, FolderId parent);

        friend class Library;
        friend class LibraryFolder;
        friend class Symbol;

    public:
        AnimFile();

        /**
         * @brief Sets the width of the document
         * @param width New width of the document, in pixels
         */
        void set_width(uint64_t width);
        /**
         * @brief Sets the height of the document
         * @param height New height of the document in pixels
         */
        void set_height(uint64_t height);
        /**
         * @brief Sets the width and height of the document
         * @param width New width of the document
         * @param height New height of the document
         */
        void set_size(uint64_t width, uint64_t height);
        /**
         * @brief sets FPS of the document
         * @param fps New FPS (Frames per Second) of the document
         */
        void set_fps(uint64_t fps);

        /**
         * @brief Adds a library to the document
         * @param group The 'group' of the newly added library
         */
        void add_library(::std::string group);

        /**
         * @brief Finds a symbol by its 'path'
         * @param path The 'path' of the symbol to be searched for
         * @return Either a @link SymbolId @endlink, or an error
         *
         * Attempts to find the symbol in the library tree referred to by the 'path'. If the attempt to
         * find the symbol fails in some way, a @link FindFailure @endlink will be returned. Upon
         * success, returns the Id of the found folder.
         */
        ::std::expected<SymbolId, FindFailure> find_symbol(::std::string path);
        /**
         * @brief Finds a folder by its 'path'
         * @param path The 'path' of the folder to be found
         * @return Either a @link FolderId @endlink, or an error
         *
         * Attempts to find the folder in the library tree referred to by the 'path'. If the attempt
         * to find the symbol fails in some way, a `FindFailure` will be returned. Upon
         * success, returns the Id of the found folder.
         */
        ::std::expected<FolderId, FindFailure> find_folder(::std::string path);
        /**
         * @brief Finds a library by its 'group'
         * @param group The 'group' of the library to be found
         * @return Either a @link LibraryId @endlink, or an error
         *
         * Attempts to find the library with the group referred to with the group parameter. If the
         * library does not exist, returns a `FindFailure::NoSuchPath`, and if the library does
         * exist, returns it's Id.
         */
        ::std::expected<LibraryId, FindFailure> get_library(::std::string group);

        /**
         * @brief Resolves a @link SymbolId @endlink to its requisite symbol.
         * @param id Id of the symbol to be resolved
         * @return Either a pointer to the resolved @link Symbol @endlink or a @link ResolveFailure @endlink
         *
         * Attempts to use the file's Symbol index to find the symbol with this Id. If the file
         * embedded in the Id does not match the value of `this`, a `ResolveFailure::WrongFile` will
         * be returned. If the Id is null, a `ResolveFailure::InvalidId` is returned. If the index is
         * out of bounds, or the object being resolved has been marked for deletion, a `ResolveFailure::NoSuchObject`
         * is returned. Upon success, a pointer to the resolved Symbol is returned.
         */
        ::std::expected<Symbol*, ResolveFailure> resolve_symbol(SymbolId id);
        /**
         * @brief Resolves a @link FolderId @endlink to its requisite folder
         * @param id Id of the folder to be resolved
         * @return Either a pointer to the resolved @link LibraryFolder @endlink or a @link ResolveFailure @endlink
         *
         * Attempts to use the file's LibraryFolder index to find the folder with this Id. If the file
         * embedded in the Id does not match the value of `this`, a `ResolveFailure::WrongFile` will
         * be returned. If the Id is null, a `ResolveFailure::InvalidId` is returned. If the index is
         * out of bounds, or the object being resolved has been marked for deletion, a `ResolveFailure::NoSuchObject`
         * is returned. Upon success, a pointer to the resolved LibraryFolder is returned.
         */
        ::std::expected<LibraryFolder*, ResolveFailure> resolve_folder(FolderId id);
        /**
         * @brief Resolves a @link LibraryId @endlink to its requisite folder
         * @param id Id of the folder to be resolved
         * @return Either a pointer to the resolved @link Library @endlink or an error
         *
         * Attempts to use the file's Library index to find the symbol with this Id. If the file
         * embedded in the Id does not match the value of `this`, a `ResolveFailure::WrongFile` will
         * be returned. If the Id is null, a `ResolveFailure::InvalidId` is returned. If the index is
         * out of bounds, or the object being resolved has been marked for deletion, a `ResolveFailure::NoSuchObject`
         * is returned. Upon success, a pointer to the resolved Library is returned.
         */
        ::std::expected<Library*, ResolveFailure> resolve_library(LibraryId id);
    };
}
