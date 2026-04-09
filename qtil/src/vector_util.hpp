//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 15 Mar 2026.
//

#pragma once

#include <vector>

namespace qtil {
    template<typename T>
    ::std::vector<T>::iterator iter_at(::std::vector<T>& vec, size_t index) {
        auto it = vec.begin();

        ::std::advance(it, index);

        return it;
    }

    template<typename T>
    class VecIterable {
        ::std::vector<T>& vec_;

    public:
        VecIterable(std::vector<T>& vec) : vec_(vec) {}

        auto begin() { return vec_.begin(); }
        auto end() { return vec_.end(); }
    };
}
