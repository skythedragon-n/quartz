//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#pragma once
#include <string>

#include "LibraryFolder.hpp"


namespace quartz::renderer {
    class Library {
        LibraryFolder root_;

        ::std::string group_;

        AnimFile* file_ = nullptr;

        struct CtorKey {
        private:
            CtorKey() = default;
            friend class AnimFile;
        };

        friend class AnimFile;
        friend class LibraryFolder;

    public:

        Library(CtorKey, ::std::string group, LibraryId id, AnimFile* file);

        Library() = delete;

        void add_symbol(::std::string name, Symbol::Type type);
        void add_folder(::std::string name);

        FindResult<SymbolId> find_symbol(::std::string path);
        FindResult<FolderId> find_folder(::std::string path);

        [[nodiscard]] ::std::string group() const { return group_; }
    };
}
