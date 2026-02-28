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

    enum class AddFailure {
        NameInUse
    };
}