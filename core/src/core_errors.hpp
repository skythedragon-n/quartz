//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Feb 2026.
//

#pragma once

#include <variant>

/**
 * @brief Contains errors relavant to the 'core' lib
 */
namespace quartz::core {
    using success_t = ::std::monostate;

    /**
     * @brief represents an error finding a symbol or folder
     *
     * An enum with variants to represent the various ways you can fail to find a resource in the file
     *
     * Variants:
     *     - NoSuchPath: The path is not present in the tree
     *     - InvalidPath: The path is poorly formatted
     */
    enum class FindFailure {
        NoSuchPath,
        InvalidPath
    };

    /**
     * @brief Represents a failure to move a symbol or folder
     *
     * An enum with variants for each way a symbol can fail to be moved.
     *
     * Variants:
     *      - NoSuchSource: The thing to be moved is non-existent
     *      - NoSuchDestination: The destination folder is non-existent
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
     *      - InvalidId: The Id's file is `nullptr`
     *      - WrongFile: the Id is being resolved with the wrong file
     *      - NoSuchObject: that there isn't an item in the file index for that object.
     *      - TagetDeleted: the Id once pointed its target, but target has since been deleted
     */
    enum class ResolveFailure {
        InvalidId,
        WrongFile,
        NoSuchObject,
        TargetDeleted
    };

    /**
     * @brief Represents a failure to add an object to a LibraryFolder or Library
     *
     * An enum with variants to represent various possible failures to add something to a LibraryFolder or Library
     *
     * Variants:
     *     - NameInUse: the name has well, been used
     */
    enum class AddFailure {
        NameInUse
    };

    /**
     * @brief Represents a failure to perform a shallow rename
     *
     * An enum with variants to represent various possible ways a shallow rename can fail
     *
     * Variants:
     *     - SourceNonexistent: The thing that you are trying to rename actually doesn't exist in the target folder
     *     - NameInUse: The new name to be used is already in use
     */
    enum class RenameFailure {
        SourceNonexistent,
        NameInUse
    };
}