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

/**
 * @file libraryutils.hpp
 *
 * @authors SkyTheDragon
 *
 * Contains various utilities for the library system, along with all Id types.
 */
namespace quartz::core {
    class AnimFile;
    class LibraryFolder;
    class Library;
    class Symbol;

    /**
     * @brief An id for a @link Symbol @endlink
     *
     * The `id` parameter is an index into the id's file's symbol index,
     * while the `file` parameter is a pointer to the Id's file
     */
    struct SymbolId {size_t id; AnimFile* file = nullptr; };
    /**
     * @brief An id for a @link Instance @endlink
     *
     * The `id` parameter is an index into the id's file's instance index,
     * while the `file` parameter is a pointer to the Id's file
     */
    struct InstanceId { size_t id; AnimFile* file = nullptr; };
    /**
     * @brief An id for a @link LibraryFolder @endlink
     *
     * The `id` parameter is an index into the id's file's folder index,
     * while the `file` parameter is a pointer to the Id's file
     */
    struct FolderId { size_t id; AnimFile* file = nullptr; };
    /**
     * @brief An id for a @link Library @endlink
     *
     * The `id` parameter is an index into the id's file's instance index,
     * while the `file` parameter is a pointer to the Id's file
     */
    struct LibraryId { size_t id; AnimFile* file = nullptr; };
    //struct FrameId { size_t id; AnimFile* file = nullptr; };
    //struct AnimLayerId { size_t id; AnimFile* file = nullptr; };

    bool operator==(const SymbolId& lhs, const SymbolId& rhs);
    bool operator==(const InstanceId& lhs, const InstanceId& rhs);
    bool operator==(const FolderId& lhs, const FolderId& rhs);
    bool operator==(const LibraryId& lhs, const LibraryId& rhs);
    //bool operator==(const FrameId& lhs, const FrameId& rhs);
    //bool operator==(const AnimLayerId& lhs, const AnimLayerId& rhs);

    constexpr SymbolId SYMBOL_ID_INVALID = {size_t(-1)};
    constexpr InstanceId INSTANCE_ID_INVALID = {size_t(-1)};
    constexpr FolderId FOLDER_ID_INVALID = {size_t(-1)};
    constexpr LibraryId LIBRARY_ID_INVALID = {size_t(-1)};
    //constexpr FrameId FRAME_ID_INVALID = {size_t(-1)};
    //constexpr AnimLayerId ANIM_LAYER_ID_INVALID = {size_t(-1)};

    using success_t = ::std::monostate;

    /**
     * @brief represents an error finding a symbol or folder
     */
    enum class FindFailure {
        NoSuchPath,
        InvalidPath
    };

    /**
    * @deprecated Gonna be replaced with `::std::expected` soon
    */
    template<typename T>
    using ActionResult = ::std::variant<success_t, T>;

    /**
     * @brief Represents a failure to move a symbol or folder
     *
     * An enum with variants for each way a symbol can fail to be moved.
     *
     * Variants:
     *      - NoSuchSource means the thing to be moved is non-existent
     *      - NoSuchDestination meaning the destination folder is non-existent
     */
    enum class MoveFailure {
        NoSuchSource,
        NoSuchDestination
    };

    /**
     * @brief Represents a failure to resolve a symbol
     *
     * An enum with variants for each way an object resolution can fail
     *
     * Variants:
     *      - InvalidId means the Id's file is `nullptr`
     *      - WrongFile means the Id is being resolved with the wrong file
     *      - NoSuchObject means that there isn't an item in the file index for that object.
     */
    enum class ResolveFailure {
        InvalidId,
        WrongFile,
        NoSuchObject
    };

    /**
     * @brief a convenience function that asks an Id's bundled file to resolve it.
     *
     * @param id the Id to be resolved
     * @return A pointer to the Symbol on success, and an error type on failure
     *
     * See @link AnimFile @endlink for more details
     */
    ::std::expected<Symbol*, ResolveFailure> resolve(SymbolId id);

    /**
     * @brief a convenience function that asks an Id's bundled file to resolve it.
     *
     * @param id the Id to be resolved
     * @return A pointer to the LibraryFolder on success, and an error type on failure
     *
     * See @link AnimFile @endlink for more details
     */
    ::std::expected<LibraryFolder*, ResolveFailure> resolve(FolderId id);

    /**
     * @brief a convenience function that asks an Id's bundled file to resolve it.
     *
     * @param id the Id to be resolved
     * @return A pointer to the Library on success, and an error type on failure
     *
     * See @link AnimFile @endlink for more details
     */
    ::std::expected<Library*, ResolveFailure> resolve(LibraryId id);
}
