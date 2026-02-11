//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <cstdint>
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
     * AnimationFile is the primary interface for managing the data of
     * animations in Quartz Renderer
     */
    class AnimFile {
        ::std::vector<Library> libraries_;
        ::std::vector<Symbol> symbols_;
        ::std::vector<LibraryFolder> folders_;

        ::std::unordered_map<std::string, LibraryId> libraries_by_group_;

        uint64_t width_ = 0, height_ = 0;
        uint64_t fps_ = 0;

        SymbolId add_symbol(std::string name, Symbol::Type type, LibraryFolder* parent);
        FolderId add_folder(std::string name, LibraryFolder* parent);

        friend class Library;
        friend class LibraryFolder;
        friend class Symbol;

    public:
        AnimFile();

        void set_width(uint64_t width);
        void set_height(uint64_t height);
        void set_size(uint64_t width, uint64_t height);
        void set_fps(uint64_t fps);

        void add_library(std::string group);

        FindResult<SymbolId> find_symbol(std::string path);
        FindResult<FolderId> find_folder(std::string path);
        FindResult<LibraryId> get_library(std::string group);

        Symbol&        resolve_symbol (size_t id) { return symbols_  [id]; }
        LibraryFolder& resolve_folder (size_t id) { return folders_  [id]; }
        Library&       resolve_library(size_t id) { return libraries_[id]; }
    };
}
