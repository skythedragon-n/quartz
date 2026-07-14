//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Jul 2026.
//

#include "Fixed32x32.hpp"

#include "panic.hpp"

Fixed32x32::Fixed32x32() : mantissa_(0) {}
Fixed32x32::Fixed32x32(int32_t value) : mantissa_(value * SCALE) {}
Fixed32x32::Fixed32x32(int64_t mantissa) : mantissa_(mantissa) {}

Fixed32x32 Fixed32x32::from_mantissa(int64_t mantissa) {
    return Fixed32x32(mantissa);
}

int64_t Fixed32x32::mantissa_value() const {
    return mantissa_;
}

Fixed32x32 Fixed32x32::operator+(const Fixed32x32& o) const {
    return from_mantissa(this->mantissa_ + o.mantissa_);
}

Fixed32x32 Fixed32x32::operator-(const Fixed32x32& o) const {
    return from_mantissa(this->mantissa_ - o.mantissa_);
}

Fixed32x32 Fixed32x32::operator*(const Fixed32x32& o) const {
    __int128_t t = static_cast<__int128>(mantissa_) * o.mantissa_;
    return from_mantissa(static_cast<int64_t>(t >> F));
}

Fixed32x32 Fixed32x32::operator/(const Fixed32x32& o) const {
    if (o.mantissa_ == 0) {
        ::qtil::panic("Divide by zero.");
    }

    __int128 t = static_cast<__int128>(mantissa_) << F;
    return from_mantissa(static_cast<int64_t>(t / o.mantissa_));
}

Fixed32x32 Fixed32x32::operator%(const Fixed32x32& o) const {
    if (o.mantissa_ == 0) {
        ::qtil::panic("Divide by zero.");
    }

    return from_mantissa(mantissa_ % o.mantissa_);
}

Fixed32x32& Fixed32x32::operator+=(const Fixed32x32& o) {
    mantissa_ += o.mantissa_;

    return *this;
}

Fixed32x32& Fixed32x32::operator-=(const Fixed32x32& o) {
    mantissa_ -= o.mantissa_;

    return *this;
}

Fixed32x32& Fixed32x32::operator*=(const Fixed32x32& o) {
    *this = *this * o;

    return *this;
}

Fixed32x32& Fixed32x32::operator/=(const Fixed32x32& o) {
    *this = *this / o;

    return *this;
}

Fixed32x32& Fixed32x32::operator%=(const Fixed32x32& o) {
    if (o.mantissa_ == 0) {
        ::qtil::panic("Divide by zero.");
    }

    mantissa_ %= o.mantissa_;

    return *this;
}
