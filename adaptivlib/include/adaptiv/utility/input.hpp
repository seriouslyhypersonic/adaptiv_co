/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_INPUT_HPP
#define ADAPTIV_INPUT_HPP

// TODO Use Boost.Spirit X3 instead of Qi

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <type_traits>

#include <adaptiv/macros.hpp>
#include <adaptiv/traits/traits.hpp>
#include <adaptiv/utility/output.hpp>
#include <adaptiv/system/system.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

// Experimental
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <adaptiv/utility/input/numeric_parsers.hpp>
#include <adaptiv/utility/input/auxiliary_parsers.hpp>
#include <adaptiv/utility/input/character_parsers.hpp>

/// Static assertion: assert type \c T is std::vector
#define ADAPTIV_ASSERT_STD_VECTOR(T, ...)       \
static_assert(                                  \
    traits::is_specialization_v<T, std::vector>,\
    __VA_ARGS__)                                \

/// Static assertion: assert type \c T is arithmetic or std::string
#define ADAPTIV_ASSERT_ARITHMETIC_OR_STRING(T, ...)         \
static_assert(                                              \
    std::is_arithmetic_v<T> || traits::is_basic_string_v<T>,\
    __VA_ARGS__)                                            \

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN

namespace input {

namespace experimental {

/// X3 parsers using adaptiv-specific grammars
namespace parsers {

namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

/// The error handler used to report a parsing failure
struct ErrorHandler
{
    template<class Iterator, class Exception, class Context>
    x3::error_handler_result on_error(
        Iterator& first,
        Iterator const& last,
        Exception const& exception,
        Context const& context)
    {
        // Get the error handler from the parser context
        auto& error_handler = x3::get<x3::error_handler_tag>(context).get();

        // Pass the position and error message
        std::string message =
            "parse error: " + exception.which() + " here:";
        error_handler(exception.where(), message);

        // The parser will quit and fail immediately
        return x3::error_handler_result::fail;
    }
};

namespace detail {
// Only inherit the ErrorHandler when required
template<bool withErrorHandling>
struct make_tag { };

template<>
struct make_tag<true>: ErrorHandler { };
} // namespace detail

/**
 * Make an X3 parser using a grammar for comma separated elements
 * @tparam T The type list elements
 * @tparam withErrorHandling Flag used to provide error handling support
 * @param first The beginning of the data to parse
 * @param last The end of the data to parse
 * @param errorOut An output stream to log any parsing errors
 * @return The X3 parser according to the list element type
 * @note Numeric types are parsed according to their specific type
 */
template<class T, bool withErrorHandling = true>
auto makeListParser()
{
    ADAPTIV_ASSERT_ARITHMETIC_OR_STRING(T,
        "list elements must be an arithmetic type or a std::string");

    struct list_tag: detail::make_tag<withErrorHandling> { };

    using x3_parsers::eolist;
    using x3_parsers::comma;

    if constexpr (std::is_arithmetic_v<T>) {
        // The numeric parser for type T
        using num_tag  = typename x3_parsers::NumericParser<T>::tag;
        auto  num_name = x3_parsers::NumericParser<T>::name;
        auto  num_def  = x3_parsers::NumericParser<T>::parser;

        auto const num = x3::rule<num_tag, T> {num_name}
                       = num_def;

        // The parser for a numeric list
        auto const list
            = x3::rule<list_tag, std::vector<T>> {"numeric list"}
            = x3::expect[num] > (eolist | (*(comma > num) > eolist));

        return list;
    } else {
        // The quoted string parser
        using x3_parsers::quotedString;

        // The parser for a list of quoted strings
        auto const list
            = x3::rule<list_tag, std::vector<T>> {"quoted string list"}
            = x3::expect[quotedString] >
                (eolist | (*(comma > quotedString) > eolist));

        return list;
    }
}

template<class T, bool withErrorHandling = true>
auto makeArrayParser()
{
    ADAPTIV_ASSERT_ARITHMETIC_OR_STRING(T,
        "array elements must be an arithmetic type or a std::string");

    struct array_tag: detail::make_tag<withErrorHandling> { };

    using x3_parsers::obracket;
    using x3_parsers::cbracket;

    if constexpr (std::is_arithmetic_v<T>) {
        // The numeric parser for type T
        using num_tag  = typename x3_parsers::NumericParser<T>::tag;
        auto  num_name = x3_parsers::NumericParser<T>::name;
        auto  num_def  = x3_parsers::NumericParser<T>::parser;

        auto const num = x3::rule<num_tag, T> {num_name}
                       = num_def;

        // The parser for a numeric array
        auto const array
            = x3::rule<array_tag, std::vector<T>> {"numeric array"}
            = x3::expect[obracket] > *num > cbracket;

        return array;
    } else {
        // The quoted string parser
        using x3_parsers::quotedString;

        // The parser for an array of quoted strings
        auto const array
            = x3::rule<array_tag, std::vector<T>> {"quoted string array"}
            = x3::expect[obracket] > *quotedString  > cbracket;

        return array;
    }
}

} // namespace parsers

enum class X3ParserType
{
    List,
    Array
};

} // namespace experimental

namespace qi = boost::spirit::qi;       // The parser library
namespace ascii = boost::spirit::ascii;
namespace classic = boost::spirit::classic;
using ascii::space;                     // A space parser

/**
 * A grammar for comma separated elements
 * @tparam T The type of the elements
 * @tparam Iterator Input iterator
 * @note Integral types are parsed as \c long or \c unsigned \c long; floating
 * point types are parsed as \c double; strings are parsed as \c std::string
 */
template<class T, class Iterator>
struct [[deprecated("Switching to Spirit.X3")]] ListParser:
    qi::grammar<Iterator, std::vector<T>(), ascii::space_type>
{
    ListParser(): ListParser::base_type(start_)
    {
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_unsigned_v<T>) {
                start_ = qi::eps > qi::ulong_ > &(qi::char_(',')) >
                    *(',' > qi::ulong_ > &(',' > qi::ulong_ | qi::eoi));
            } else {
                start_ = qi::eps > qi::long_ > &(qi::char_(',')) >
                    *(',' > qi::long_ > &(',' > qi::long_ | qi::eoi));
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            start_ = qi::eps > qi::double_ > &(qi::char_(',')) >
                *(',' > qi::double_ > &(',' > qi::double_ | qi::eoi));
        } else if constexpr (traits::is_basic_string_v<T>) {
            quotedString_ %= qi::lexeme['"' > + (qi::char_ - '"') > '"'];
            start_ = qi::eps > quotedString_ > &(qi::char_(',')) >
                *(',' > quotedString_ > &((',' > quotedString_) | qi::eoi));
        } else {
            static_assert(
                std::is_arithmetic_v<T> || traits::is_basic_string_v<T>,
                    "[adaptiv::input::ArrayParser] requires an arithmetic "
                    "or string type");
        }
    }

    qi::rule<Iterator, std::string(), ascii::space_type> quotedString_;
    qi::rule<Iterator, std::vector<T>(), ascii::space_type> start_;
};

/**
 * A grammar for arrays: [elem1 elem2 ... elemN]
 * @tparam T The type of array elements
 * @tparam Iterator Input iterator
 * @note Integral types are parsed as \c long or \c unsigned \c long; floating
 * point types are parsed as \c double; strings are parsed as \c std::string
 */
template<class T, class Iterator>
struct [[deprecated("Switching to Spirit.X3")]] ArrayParser:
    qi::grammar<Iterator, std::vector<T>(), ascii::space_type>
{
    ArrayParser(): ArrayParser::base_type(start_)
    {
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_unsigned_v<T>) {
                start_ = qi::eps > '[' > *qi::ulong_ > ']';
            } else {
                start_ = qi::eps > '[' > *qi::long_ > ']';
            }
        } else if constexpr (std::is_floating_point_v<T>) {
            start_ = qi::eps > '[' > *qi::double_ > ']';
        } else if constexpr (traits::is_basic_string_v<T>) {
            quotedString_ = qi::lexeme['"' > + (qi::char_ - '"') > '"'];
            start_ = qi::eps > '[' > *quotedString_ > ']';
        } else {
            static_assert(
            std::is_arithmetic_v<T> || traits::is_basic_string_v<T>,
            "[adaptiv::input::ArrayParser] requires an arithmetic "
            "or string type");
        }
    }

    qi::rule<Iterator, std::string(), ascii::space_type> quotedString_;
    qi::rule<Iterator, std::vector<T>(), ascii::space_type> start_;
};

/**
 * @enum Describes the grammar used by the parser
 */
enum class ParserType
{
    /**
     * A comma separated list of elements.
     * @note The container passed to the parser must be a std::vector and its
     * elements must be of an arithmetic or string type type.
     * @note Parsing negative values into a container of unsigned values fails
     * @note Strings must be enclosed in double quotes
     */
    List,

    /**
     * A space separated array of elements enclosed in brackets.
     * @note The container passed to the parser must be a std::vector and its
     * elements must be of an arithmetic or string type.
     * @note Parsing negative values into a container of unsigned values fails
     * @note Strings must be enclosed in double quotes
     */
    Array
};

/**
 * A simple line parser with various grammars
 */
class LineParser
{
    using iterator_type = std::string::iterator; ///< Parser iterator

    /// Position iterator providing access to column numbers
    using pos_iterator_type = classic::position_iterator2<iterator_type>;

    std::istream& in_;  ///< The istream to read data from
    std::string data_;  ///< A line of data
    std::string error_; ///< Store an error message if parsing fails

    /**
     * Parse a line using a specific grammar and store the resulting attributes
     * in a container.
     */
    template<class Parser, class ResultContainer>
    [[deprecated("Switching to Spirit.X3")]] bool doParse(Parser& parser, ResultContainer& container)
    {
        pos_iterator_type positionBegin(data_.begin(), data_.end());
        pos_iterator_type positionEnd;

        bool parsingSucceeded; // Check if any parser components failed
        auto first = data_.begin();
        auto last = data_.end();

        try {
            qi::phrase_parse(
                positionBegin,
                positionEnd,
                parser,
                ascii::space,
                container);
        } catch (qi::expectation_failure<pos_iterator_type> const& exception) {
            classic::file_position_base<std::string> const& position =
                exception.first.get_position();
            std::stringstream message;

            namespace style = adaptiv::utility::output::style;
            message << style::error << "error: " << style::none <<
                "parse error on column " << position.column << "\n" <<
                '\'' << exception.first.get_currentline() << "'\n" <<
                style::error <<
                std::string(position.column, '~') << "^~~~ here";
            error_ = message.str();
            return false;
        }

        return true;
    }

    // Experimental
    template<class Parser, class ParserAttribute>
    bool doX3Parse(Parser parser, ParserAttribute& attribute)
    {
        namespace x3 = boost::spirit::x3;
        namespace ascii = boost::spirit::x3::ascii;

        using list_value_type = typename ParserAttribute::value_type;

        // Tag to access get a reference to the actual x3::error_handler
        using x3::error_handler_tag;

        using error_handler_type = x3::error_handler<iterator_type>;

        // The error handler
        std::stringstream error;
        error_handler_type errorHandler(data_.begin(), data_.end(), error);

        // The parser with the error handler injected via with directive
        auto const parserWithErrorHandling =
            x3::with<error_handler_tag>(std::ref(errorHandler))
                [
                    parser
                ];

        bool isParsed = x3::phrase_parse(
            data_.begin(),
            data_.end(),
            parserWithErrorHandling,
            ascii::space,
            attribute);

        // Get the error message
        if (!isParsed) {
            std::string line;
            error_.clear();

            for (int i = 0; std::getline(error, line); ++i) {
                if (i != 0) { // Discard expectation_failure line
                    if (i == 1) {
                        error_.append(line);
                    } else {
                        error_.append('\n' + line);
                    }
                }
            }
        }

        return isParsed;
    }

public:
    explicit LineParser(std::istream& in) : in_(in)
    { }

    /**
     * Parses a line using the grammar specified by ParserType
     * @tparam ParserType An enumerator specifying the grammar used for parsing
     * @tparam ResultContainer A container type compatible with the \c
     * ParserType. Please check the documentation of \c ParserType enumerators
     * for details on container compatibility.
     * @return \c true if parsing succeeded, \c false otherwise.
     */
    template<class ResultContainer>
    [[deprecated("Switching to Spirit.X3")]] bool parse(ParserType parserType, ResultContainer& container)
    {
        if (data_.empty()) {
            std::cerr << "no data to parse\n";
            return false;
        }

        switch (parserType) {
            case ParserType::List:
            {
                using value_type = typename ResultContainer::value_type;
                ListParser<value_type, pos_iterator_type> listParser;
                return doParse(listParser, container);
            }
            case ParserType::Array:
            {
                using value_type = typename ResultContainer::value_type;
                ArrayParser<value_type, pos_iterator_type> arrayParser;
                return doParse(arrayParser, container);
            }
        }
    }

    // Experimental
    template<class ParserAttribute>
    bool parse(experimental::X3ParserType parserType, ParserAttribute& attribute)
    {
        if (data_.empty()) {
            adaptiv::fail("no data to parse");
            return false;
        }

        switch (parserType) {
            case experimental::X3ParserType::List:
            {
                ADAPTIV_ASSERT_STD_VECTOR(
                    ParserAttribute,
                    "the ParserAttribute for a List parser "
                    "must be a std::vector");

                using list_value_type = typename ParserAttribute::value_type;
                auto const listParser =
                    experimental::parsers::makeListParser<list_value_type>();

                return doX3Parse(listParser, attribute);
            }
            case experimental::X3ParserType::Array:
            {
                ADAPTIV_ASSERT_STD_VECTOR(
                ParserAttribute,
                "the ParserAttribute for an Array parser "
                "must be a std::vector");

                using array_value_type = typename ParserAttribute::value_type;
                auto const arrayParser =
                           experimental::parsers::makeArrayParser<array_value_type>();

                return doX3Parse(arrayParser, attribute);
            }
        }
    }

    /**
     * Returns the first word in the sequence delimited to the right by
     * \c whitespace.
     * @param whitespace The characters to be interpreted as space.
     * @return The first word in the sequence.
     */
    std::string firstWord(std::string const& whitespace = " \t")
    {
        return data_.substr(0, data().find_first_of(whitespace));
    }

    /**
     * Checks if the sequence starts with the prefix specified by \c word and
     * delimited to the right by \c whitespace.
     * @param word The prefix to test against.
     * @param whitespace The characters to be interpreted as spaces.
     * @return \c true if the prefix is found, \c false otherwise.
     */
    bool startsWith(
        std::string const& word,
        std::string const& whitespace = " \t")
    {

        return firstWord(whitespace) == word;
    }

    /**
     * Checks if the sequence starts with the prefix specified by \c word and
     * delimited to the right by \c whitespace. Elements are compared case
     * insensitively.
     * @param word The prefix to test against.
     * @param whitespace The characters to be interpreted as spaces.
     * @return \c true if the prefix is found, \c false otherwise.
     */
    bool iStartsWith(
        std::string const& word,
        std::string const& whitespace = " \t")
    {

        return boost::iequals(firstWord(whitespace),  word);
    }

    /// Remove first word from the loaded data
    std::string popFront()
    {
        auto front = firstWord();
        boost::erase_first(data_, firstWord());
        boost::trim_left(data_);
        return front;
    }

    /// Prompt input showing an optional message prepended and get the line data
    std::istream& getline(std::string const& prompt = "")
    {
        if (!prompt.empty()) {
            // Only access cout if there is something to output
            std::cout << prompt << " ";
        }

        // Get line and discard any prefixed whitespace
        std::getline(in_, data_);
        boost::trim_left(data_);

        return in_;
    }

    /// Raw line loaded into the parser using \c getline
    std::string const& data() const
    {
        return data_;
    }

    /// Return the error message
    std::string const& error() const
    {
        return error_;
    }

    /// Output error message to cerr
    void fail() const
    {
        adaptiv::fail(error_);
    }
};

} //namespace input

ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_INPUT_HPP
