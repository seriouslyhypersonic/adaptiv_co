/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_SERIALIZATION_MACROS_HPP
#define ADAPTIV_SERIALIZATION_MACROS_HPP

#include <boost/preprocessor.hpp>

// detail: register class members for archival
#define ADAPTIV_ARCHIVE_ELEM_impl(r, data, elem) CEREAL_NVP(elem)

/**
 * @note Make class members serializable using cereal's facilities.
 * @details This macro adds the simple serialization method bellow and
 * registers its arguments (i.e. the class members) for archival.
 * The class members are archived using their variable name as the identifier.
 * @code
 *     template<class Archive>
 *     void serialize(Archive& archive)
 *     {
 *          archive(member1, member2, ...);
 *     }
 * @endcode
 */
#define ADAPTIV_SERIALIZE(...)                          \
template<class Archive>                                 \
void serialize(Archive& archive)                        \
{                                                       \
    archive(                                            \
        BOOST_PP_SEQ_ENUM(                              \
            BOOST_PP_SEQ_TRANSFORM(                     \
                ADAPTIV_ARCHIVE_ELEM_impl,              \
                data,                                   \
                BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))) \
                                                        \
    );                                                  \
} struct {}                                             \

#endif //ADAPTIV_SERIALIZATION_MACROS_HPP
