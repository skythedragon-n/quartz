//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Mar 2026.
//

#pragma once

namespace qtil {
    template<typename T, typename U>
    constexpr T pun_cast(U&& u) noexcept {
        static_assert(sizeof(T) == sizeof(U), "Type punning requires input and output types to be the same size");
        //static_assert(std::is_trivially_copyable_v<T> && std::is_trivially_copyable_v<U>,
        //          "pun_cast requires trivially copyable types");
        return ::std::bit_cast<T>(::std::forward<U>(u));
    }
}
