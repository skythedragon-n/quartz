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
     * @class AnimFile
     * @brief Root of internal representation of animations
     *
     * @authors SkyTheDragon
     *
     * AnimationFile is the primary interface for managing the data of
     * animations in Quartz Renderer
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
         */
        SymbolId add_symbol(::std::string name, Symbol::Type type, FolderId parent);
        /**
         * @brief Adds a new @link LibraryFolder @endlink to the document
         * @param name Name of the folder to be added
         * @param parent The parent folder of this folder
         * @return The Id of the newly minted folder
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
         */
        ::std::expected<SymbolId,  FindFailure> find_symbol(::std::string path);
        /**
         * @brief Finds a folder by its 'path'
         * @param path The 'path' of the folder to be found
         * @return Either a @link FolderId @endlink, or an error
         */
        ::std::expected<FolderId,  FindFailure> find_folder(::std::string path);
        /**
         * @brief Finds a library by its 'group'
         * @param group The 'group' of the library to be found
         * @return Either a @link LibraryId @endlink, or an error
         */
        ::std::expected<LibraryId, FindFailure> get_library(::std::string group);

        /**
         * @brief Resolves a @link SymbolId @endlink to its requisite symbol.
         * @param id Id of the symbol to be resolved
         * @return Either a pointer to the resolved @link Symbol @endlink or an error
         */
        ::std::expected<Symbol*,        ResolveFailure> resolve_symbol  (SymbolId  id);
        /**
         * @brief Resolves a @link FolderId @endlink to its requisite folder
         * @param id Id of the folder to be resolved
         * @return Either a pointer to the resolved @link LibraryFolder @andlink or an error
         */
        ::std::expected<LibraryFolder*, ResolveFailure> resolve_folder  (FolderId  id);
        /**
         * @brief Resolves a @link LibraryId @endlink to its requisite folder
         * @param id Id of the folder to be resolved
         * @return Either a pointer to the resolved @link Library @andlink or an error
         */
        ::std::expected<Library*,       ResolveFailure> resolve_library (LibraryId id);
    };
}
