//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/4/26.
//

#include <memory>
#include <string>

namespace quartz::renderer {
    struct Document {};


    void init() {}
    std::unique_ptr<Document> parse(const std::string& path) { return {}; }
    void render(const Document& render) {}
}
