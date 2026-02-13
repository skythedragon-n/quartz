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
#include "Symbol.hpp"

namespace quartz::renderer {
    class AnimFile;

    class LibraryFolder {
        ::std::unordered_map<::std::string, SymbolId> symbols_;
        ::std::unordered_map<::std::string, FolderId> folders_;

        ::std::string name_;

        std::variant<LibraryId, FolderId> parent_;

        AnimFile* file_ = nullptr;

        const FolderId id_;

        void set_parent(LibraryId parent);

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

        void set_name(::std::string name);

        void add_symbol(::std::string name, Symbol::Type type);
        void add_folder(::std::string name);

        FindResult<SymbolId> find_symbol(std::string path);
        FindResult<FolderId> find_folder(std::string path);

        [[nodiscard]] ::std::string name() const { return name_; }
    };
}
