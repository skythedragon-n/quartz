//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/9/26.
//

#pragma once

#include <optional>

namespace quartz::renderer {
    class AnimFile;
    class LibraryFolder;
    class Library;

    struct SymbolId { size_t id; };
    struct FolderId { size_t id; };
    struct LibraryId { size_t id; };

    constexpr SymbolId SYMBOL_ID_INVALID = {size_t(-1)};
    constexpr FolderId FOLDER_ID_INVALID = {size_t(-1)};
    constexpr LibraryId LIBRARY_ID_INVALID = {size_t(-1)};

    using success_t = std::monostate;

    template<typename T>
    using FindResult = std::variant<T, std::nullopt_t>;

    template<typename T>
    using ActionResult = std::variant<success_t, T>;

    enum class MoveFailure {
        NoSuchSource,
        NoSuchDestination
    };
}
