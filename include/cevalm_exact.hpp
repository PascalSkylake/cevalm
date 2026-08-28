// SPDX-License-Identifier: SunPro AND LGPL-2.1-or-later

/*
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 * Permission to use, copy, modify, and distribute this software is freely
 * granted, provided that this notice is preserved.
 * fdim, fmin, and fmax include material derived from GNU C Library sources
 * distributed under LGPL-2.1-or-later.
 */
#pragma once

#include "cevalm_binary64.hpp"
#include "cevalm_classify.hpp"

namespace cevalm {

consteval double fabs(double value) {
    return binary64_from_bits(binary64_bits(value) & ~binary64_sign_mask);
}

consteval double abs(double value) {
    return cevalm::fabs(value);
}

consteval double copysign(double magnitude, double sign) {
    return binary64_from_bits((binary64_bits(magnitude) & ~binary64_sign_mask) |
                              (binary64_bits(sign) & binary64_sign_mask));
}

consteval double fmin(double x, double y) {
    if (isnan(y))
        return x;
    if (isnan(x))
        return y;
    return x <= y ? x : y;
}

consteval double fmax(double x, double y) {
    if (isnan(y))
        return x;
    if (isnan(x))
        return y;
    return x >= y ? x : y;
}

consteval double fdim(double x, double y) {
    if (isnan(x) || isnan(y))
        return binary64_propagate_nan(x, y);
    return x <= y ? 0.0 : x - y;
}

} // namespace cevalm
