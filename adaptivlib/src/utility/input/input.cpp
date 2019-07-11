/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <adaptiv/utility/input.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_INPUT_NAMESPACE_BEGIN

/// Helper intantiation maker
#define ADAPTIV_INSTANTIATE_PARSE(AttributeType)                           \
    template                                                               \
    bool LineParser::parse(ParserType parserType, AttributeType& attribute)\

ADAPTIV_INSTANTIATE_PARSE(float_vector_type);
ADAPTIV_INSTANTIATE_PARSE(double_vector_type);
ADAPTIV_INSTANTIATE_PARSE(short_vector_type);
ADAPTIV_INSTANTIATE_PARSE(uint_vector_type);
ADAPTIV_INSTANTIATE_PARSE(int_vector_type);
ADAPTIV_INSTANTIATE_PARSE(long_vector_type);
ADAPTIV_INSTANTIATE_PARSE(ulong_vector_type);
ADAPTIV_INSTANTIATE_PARSE(ulonglong_vector_type);
ADAPTIV_INSTANTIATE_PARSE(string_vector_type);

ADAPTIV_INPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END
