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
#include "id_sys.hpp"
#include "core_errors.hpp"
#include "Symbol.hpp"


namespace quartz::core {
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
         * @brief Finds a library by its 'group'
         * @param group The 'group' of the library to be found
         * @return Either a @link LibraryId @endlink, or an error
         *
         * Attempts to find the library with the group referred to with the group parameter. If the
         * library does not exist, returns a `FindFailure::NoSuchPath`, and if the library does
         * exist, returns it's Id.
         */
        ::std::expected<LibraryId, FindFailure> get_library(::std::string group);

        IdContainer<Symbol> symbols;
        IdContainer<LibraryFolder> folders;
        IdContainer<Library> libraries;
    };
}
