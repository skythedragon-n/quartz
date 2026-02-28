//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 12 Feb 2026.
//

#include "id_sys.hpp"

namespace quartz::core {
    bool operator==(const SymbolId& lhs, const SymbolId& rhs) {
        return lhs.id == rhs.id && lhs.file == rhs.file;
    }

    bool operator==(const InstanceId& lhs, const InstanceId& rhs) {
        return lhs.id == rhs.id && lhs.file == rhs.file;
    }

    bool operator==(const FolderId& lhs, const FolderId& rhs) {
        return lhs.id == rhs.id && lhs.file == rhs.file;
    }

    bool operator==(const LibraryId& lhs, const LibraryId& rhs) {
        return lhs.id == rhs.id && lhs.file == rhs.file;
    }

    bool operator==(const FrameId& lhs, const FrameId& rhs) {
        return lhs.id == rhs.id && lhs.file == rhs.file;
    }
}
