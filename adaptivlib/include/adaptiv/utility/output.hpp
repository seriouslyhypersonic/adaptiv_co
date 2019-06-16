/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_OUTPUT_HPP
#define ADAPTIV_OUTPUT_HPP

#include <iostream>
#include <type_traits>

#include <adaptiv/macros.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN

namespace output {

/// Attribute "Font": a display effect or font style
enum class Font
{
    Regular       = 0,
    Bold          = 1,
    BoldOff       = 21,
    Underline     = 4,
    UnderlineOff  = 24,
    Blink         = 5,
    BlinkOff      = 25,
    Inverse       = 7,
    InverseOff    = 27
};

/// Attribute "Fg": foreground (font) color
enum class Fg
{
    Black   = 30,
    Red     = 31,
    Green   = 32,
    Yellow  = 33,
    Blue    = 34,
    Magenta = 35,
    Cyan    = 36,
    White   = 37,
    Default = 39
};

/// Attribute "Bg": background (console) color
enum class Bg
{
    Black   = 40,
    Red     = 41,
    Green   = 42,
    Yellow  = 43,
    Blue    = 44,
    Magenta = 45,
    Cyan    = 46,
    White   = 47,
    Default = 49
};

/// StyleRule is a collection of attributes that defines an output style
struct StyleRule
{
    Font font = Font::Regular;
    Fg   fg   = Fg::Default;
    Bg   bg   = Bg::Default;
};

/// Some predefined output styles
namespace style {
constexpr StyleRule none    { };
constexpr StyleRule warning {Font::Bold, Fg::Yellow, Bg::Default};
constexpr StyleRule error   {Font::Bold, Fg::Red,    Bg::Default};
constexpr StyleRule success {Font::Bold, Fg::Green,  Bg::Default};
constexpr StyleRule message {Font::Bold, Fg::Blue,   Bg::Default};
} // namespace style

namespace detail {
// Produce an ostream ref if T is an attribute (i.e. Font, Fg, or Bg)
template<class T>
using ostreamRefIfAtt = std::enable_if_t<std::is_same_v<T, Font> ||
                                         std::is_same_v<T, Fg> ||
                                         std::is_same_v<T, Bg>
                                        ,std::ostream&>;

char const* csi = "\033["; // control sequence introducer
} // namespace detail

/// Overload of operator<< for attributes
template<class T>
detail::ostreamRefIfAtt<T> operator<<(std::ostream& out, T const& attribute)
{
    return out << detail::csi << static_cast<int>(attribute) << 'm';
}

/// Overload of operator<< for a StyleRule
std::ostream& operator<<(std::ostream& out, StyleRule const& rule)
{
    return out << detail::csi <<
        static_cast<int>(rule.font) << ';' <<
        static_cast<int>(rule.fg)   << ';' <<
        static_cast<int>(rule.bg)   << 'm';
}

} // namespace output

ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_OUTPUT_HPP
