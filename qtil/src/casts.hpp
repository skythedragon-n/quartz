//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Mar 2026.
//

#pragma once

namespace qtil {
    template<typename T, typename U>
    T pun_cast(U&& u) {
        return ::std::bit_cast<T>(::std::forward<U>(u));
    }
}
