// SPDX-License-Identifier: LicenseRef-Red-Hat-FDLIBM AND LGPL-2.1-or-later

/*
 * Copyright (C) 2002, 2007 by Red Hat, Incorporated. All rights reserved.
 * Permission to use, copy, modify, and distribute this software is freely
 * granted, provided that this notice is preserved.
 */
#pragma once

#include "cevalm_binary64.hpp"

namespace cevalm {

inline constexpr int fp_nan = 0;
inline constexpr int fp_infinite = 1;
inline constexpr int fp_zero = 2;
inline constexpr int fp_subnormal = 3;
inline constexpr int fp_normal = 4;

consteval int fpclassify(double value) {
    const uint64 bits = binary64_bits(value);
    const uint64 exponent = bits & binary64_exponent_mask;
    const uint64 fraction = bits & binary64_fraction_mask;
    if (exponent == binary64_exponent_mask)
        return fraction == 0 ? fp_infinite : fp_nan;
    if (exponent == 0)
        return fraction == 0 ? fp_zero : fp_subnormal;
    return fp_normal;
}

consteval bool isfinite(double value) {
    return (binary64_bits(value) & binary64_exponent_mask) != binary64_exponent_mask;
}

consteval bool isinf(double value) {
    return binary64_is_infinite_bits(binary64_bits(value));
}

consteval bool isnan(double value) {
    return binary64_is_nan_bits(binary64_bits(value));
}

consteval bool isnormal(double value) {
    return fpclassify(value) == fp_normal;
}

consteval bool signbit(double value) {
    return (binary64_bits(value) & binary64_sign_mask) != 0;
}

consteval bool isgreater(double x, double y) {
    return !isnan(x) && !isnan(y) && x > y;
}

consteval bool isgreaterequal(double x, double y) {
    return !isnan(x) && !isnan(y) && x >= y;
}

consteval bool isless(double x, double y) {
    return !isnan(x) && !isnan(y) && x < y;
}

consteval bool islessequal(double x, double y) {
    return !isnan(x) && !isnan(y) && x <= y;
}

consteval bool islessgreater(double x, double y) {
    return !isnan(x) && !isnan(y) && x != y;
}

consteval bool isunordered(double x, double y) {
    return isnan(x) || isnan(y);
}

consteval bool issignaling(double value) {
    const uint64 bits = binary64_bits(value);
    return binary64_is_nan_bits(bits) && (bits & binary64_quiet_nan_mask) == 0;
}

} // namespace cevalm
