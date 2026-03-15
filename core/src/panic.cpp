//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Mar 2026.
//

#include "panic.hpp"

#include <iostream>

namespace qtil {
    void panic(const char* message) noexcept {
        std::cerr << "Quartz panicked: " << message << "\n";
        std::abort();
    }
}
