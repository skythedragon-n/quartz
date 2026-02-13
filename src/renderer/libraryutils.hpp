//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/9/26.
//

#pragma once

#include <optional>
#include <variant>

#include "AnimFile.hpp"

namespace quartz::renderer {
    class AnimFile;
    class LibraryFolder;
    class Library;
    class Symbol;

    struct SymbolId { size_t id; AnimFile* file = nullptr; };
    struct FolderId { size_t id; AnimFile* file = nullptr; };
    struct LibraryId { size_t id; AnimFile* file = nullptr; };

    bool operator==(const SymbolId& lhs, const SymbolId& rhs);
    bool operator==(const FolderId& lhs, const FolderId& rhs);
    bool operator==(const LibraryId& lhs, const LibraryId& rhs);

    constexpr SymbolId SYMBOL_ID_INVALID = {size_t(-1)};
    constexpr FolderId FOLDER_ID_INVALID = {size_t(-1)};
    constexpr LibraryId LIBRARY_ID_INVALID = {size_t(-1)};

    using success_t = ::std::monostate;

    template<typename T>
    using FindResult = ::std::variant<T, std::nullopt_t>;

    template<typename T>
    using ActionResult = ::std::variant<success_t, T>;

    enum class MoveFailure {
        NoSuchSource,
        NoSuchDestination
    };

    enum class ResolveFailure {
        InvalidId,
        WrongFile
    };

    inline ::std::expected<Symbol*, ResolveFailure> resolve(SymbolId id) {
        return id.file->resolve_symbol(id);
    }

    inline ::std::expected<LibraryFolder*, ResolveFailure> resolve(FolderId id) {
        return id.file->resolve_folder(id);
    }

    inline ::std::expected<Library*, ResolveFailure> resolve(LibraryId id) {
        return id.file->resolve_library(id);
    }
}
