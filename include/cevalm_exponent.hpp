// SPDX-License-Identifier: SunPro

/*
 * Binary64 exponent operations derived from fdlibm.
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Permission to use, copy, modify, and distribute this software is freely
 * granted, provided that this notice is preserved.
 */
#pragma once

#include "cevalm_binary64.hpp"
#include "cevalm_classify.hpp"
#include "cevalm_round.hpp"
#include "cevalm_scale.hpp"

namespace cevalm {

inline constexpr int fp_ilogb_zero = (-2147483647 - 1);
inline constexpr int fp_ilogb_nan = 2147483647;

consteval int ilogb(double value) {
    const uint64 magnitude = binary64_magnitude_bits(value);
    const int encoded = static_cast<int>((magnitude >> 52) & 0x7ff);
    if (encoded != 0)
        return encoded == 0x7ff ? fp_ilogb_nan : encoded - 1023;
    if (magnitude == 0)
        return fp_ilogb_zero;
    int position = 0;
    uint64 bits = magnitude;
    while ((bits >> 1) != 0) {
        bits >>= 1;
        ++position;
    }
    return position - 1074;
}

consteval double logb(double value) {
    if (isnan(value))
        return binary64_quiet_nan(value);
    if (isinf(value))
        return binary64_infinity(false);
    if (binary64_magnitude_bits(value) == 0)
        return binary64_infinity(true);
    return static_cast<double>(cevalm::ilogb(value));
}

consteval double significand(double value) {
    const uint64 bits = binary64_bits(value);
    const uint64 exponent = bits & binary64_exponent_mask;
    if (exponent != 0 && exponent != binary64_exponent_mask)
        return binary64_from_bits((bits & (binary64_sign_mask | binary64_fraction_mask)) |
                                  0x3ff0000000000000ULL);
    return value;
}

consteval double scalb(double value, double exponent) {
    if (isnan(value) || isnan(exponent))
        return binary64_propagate_nan(value, exponent);
    if (isinf(exponent)) {
        if (!signbit(exponent)) {
            if (binary64_magnitude_bits(value) == 0)
                return binary64_from_bits(0xfff8000000000000ULL);
            return isinf(value) ? value : binary64_infinity(signbit(value));
        }
        if (binary64_magnitude_bits(value) == 0)
            return value;
        if (isinf(value))
            return binary64_from_bits(0xfff8000000000000ULL);
        return binary64_signed_zero(signbit(value));
    }
    if (cevalm::rint(exponent) != exponent)
        return binary64_from_bits(0x7ff8000000000000ULL);
    if (exponent > 65000.0)
        return cevalm::scalbln(value, 65000L);
    if (exponent < -65000.0)
        return cevalm::scalbln(value, -65000L);
    return cevalm::scalbln(value, static_cast<long>(exponent));
}

} // namespace cevalm
