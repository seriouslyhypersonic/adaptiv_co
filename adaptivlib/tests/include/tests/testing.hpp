/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_TESTING_HPP
#define ADAPTIV_TESTING_HPP

#include <gtest/gtest.h>

/// Assert that elements of two std::vector are equal
#define ADAPTIV_ASSERT_ELEMENTS_EQ(vec1, vec2)              \
    do {                                                    \
        for (std::size_t i = 0; i < vec1.size(); ++i) {     \
            ASSERT_EQ(vec1[i], vec2[i]);                    \
        }                                                   \
        break;                                              \
    } while (true)  // do...while() to force semicolon      \

/// Assert that elements of two std::vector are near each other
#define ADAPTIV_ASSERT_ELEMENTS_NEAR(vec1, vec2, absError)  \
    do {                                                    \
        for (std::size_t i = 0; i < vec1.size(); ++i) {     \
            ASSERT_NEAR(vec1[i], vec2[i], absError);        \
        }                                                   \
        break;                                              \
    } while (true)  // do...while() to force semicolon      \

#endif //ADAPTIV_TESTING_HPP
