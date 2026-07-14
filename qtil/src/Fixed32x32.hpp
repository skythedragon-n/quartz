//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Jul 2026.
//

#pragma once
#include <cstdint>


class Fixed32x32 {
    static constexpr int F = 32;
    static constexpr int64_t SCALE = 1LL << F;
    int64_t mantissa_;

public:
    explicit Fixed32x32(int64_t mantissa);
    Fixed32x32();
    explicit Fixed32x32(int32_t value);

    static Fixed32x32 from_mantissa(int64_t mantissa);

    [[nodiscard]] int64_t mantissa_value() const;

    Fixed32x32 operator+(const Fixed32x32& o) const;
    Fixed32x32 operator-(const Fixed32x32& o) const;
    Fixed32x32 operator*(const Fixed32x32& o) const;
    Fixed32x32 operator/(const Fixed32x32& o) const;
    Fixed32x32 operator%(const Fixed32x32& o) const;

    Fixed32x32& operator+=(const Fixed32x32& o);
    Fixed32x32& operator-=(const Fixed32x32& o);
    Fixed32x32& operator*=(const Fixed32x32& o);
    Fixed32x32& operator/=(const Fixed32x32& o);
    Fixed32x32& operator%=(const Fixed32x32& o);
};
