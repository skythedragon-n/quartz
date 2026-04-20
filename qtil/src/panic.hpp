//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Mar 2026.
//

#pragma once

#include <string>

namespace qtil {
    /**
     * @brief Exits instantly, printing message
     * @param message Message to print
     * paniks
     */
    [[noreturn]]
    void panic(::std::string message) noexcept;
}
