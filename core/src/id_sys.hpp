//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/9/26.
//

#pragma once

#include <cstddef>

/**
 * @file id_sys.hpp
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
    class Frame;
    class AnimatedLayer;
    class Stroke;
    class Drawing;
    struct Instance;

    struct AnimKey {
    private:
        AnimKey() = default;
        friend class AnimFile;
    };

    /**
     * @page ids ID explanation
     * Id's contain two members, an `id` member, which is an index into the AnimFile's object table for that type, and
     * a `file` member, which is a pointer to the Id's file.
     *
     * DO NOT SET THESE MEMBERS UNDER ANY CIRCUMSTANCES. SUCH IS UNDEFINED BEHAVIOR, AND COULD CAUSE ANYTHING TO HAPPEN,
     * INCLUDING, BUT NOT LIMITED TO YOUR COMPUTER FILLING YOUR HOME WITH AN INORDINATE NUMBER OF PUPPIES & RAINBOWS.
     */

    struct SymbolId {size_t id; AnimFile* file = nullptr; };
    struct InstanceId { size_t id; AnimFile* file = nullptr; };
    struct FolderId { size_t id; AnimFile* file = nullptr; };
    struct LibraryId { size_t id; AnimFile* file = nullptr; };
    struct FrameId { size_t id; AnimFile* file = nullptr; };
    struct AnimLayerId { size_t id; AnimFile* file = nullptr; };

    bool operator==(const SymbolId& lhs, const SymbolId& rhs);
    bool operator==(const InstanceId& lhs, const InstanceId& rhs);
    bool operator==(const FolderId& lhs, const FolderId& rhs);
    bool operator==(const LibraryId& lhs, const LibraryId& rhs);
    bool operator==(const FrameId& lhs, const FrameId& rhs);
    bool operator==(const AnimLayerId& lhs, const AnimLayerId& rhs);

    constexpr SymbolId SYMBOL_ID_INVALID = {size_t(-1)};
    constexpr InstanceId INSTANCE_ID_INVALID = {size_t(-1)};
    constexpr FolderId FOLDER_ID_INVALID = {size_t(-1)};
    constexpr LibraryId LIBRARY_ID_INVALID = {size_t(-1)};
    constexpr FrameId FRAME_ID_INVALID = {size_t(-1)};
    constexpr AnimLayerId ANIM_LAYER_ID_INVALID = {size_t(-1)};
}
