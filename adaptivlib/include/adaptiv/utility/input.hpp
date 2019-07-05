/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef ADAPTIV_INPUT_HPP
#define ADAPTIV_INPUT_HPP

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
#include <adaptiv/utility/input/parsers.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_INPUT_NAMESPACE_BEGIN

/**
 * @enum Describes the grammar used by the parser
 */
enum class ParserType
{
    /**
     * A comma separated list of elements.
     * @note The container passed to the parser must be a std::vector and its
     * elements must be of an arithmetic or string type.
     * @note Numeric types are parsed according to their specific X3 numeric
     * parser.
     * @note Strings are parsed as quoted text.
     */
    List,

    /**
     * A space separated array of elements enclosed in brackets.
     * @note The container passed to the parser must be a std::vector and its
     * elements must be of an arithmetic or string type.
     * @note Numeric types are parsed according to their specific X3 numeric
     * parser.
     * @note Strings are parsed as quoted text.
     */
    Array
};

/**
 * A simple line parser with various grammars
 */
class LineParser
{
    using iterator_type = std::string::iterator; ///< Parser iterator

    std::istream& in_;  ///< The istream to read data from
    std::string data_;  ///< A line of data
    std::string error_; ///< Store an error message if parsing fails

    /**
     * Parse a line using a specific X3 rule that describes the \c ParserType
     * grammar. The parser is injected with an error handler so that parsing
     * errors can be reported with descriptive error messages via
     * \c LineParser::error(). The resulting AST is stored in the \c attribute.
     * @tparam Parser An X3 parser type
     * @tparam ParserAttribute A type compatible with the X3 parser and is
     * used to store the abstract syntax tree (AST) after parsing.
     * @param parser The X3 parser instance generated with
     * \c parsers::make*Parser() and error handling enabled.
     * @param attribute The instance of the \c ParserAttribute where the AST
     * will be stored.
     * @return \c true if parsing succeeded, \c false otherwise. If parsing
     * fails, a descriptive error message is available using
     * \c LineParser::error()
     */
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

        // Clear any previously generated error messages
        error_.clear();

        // Get the error message
        if (!isParsed) {
            std::string line;


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
     * @tparam ParserAttribute A type that matches the \c ParserType and is
     * used to store the abstract syntax tree (AST) after parsing. Please check
     * the documentation of \c ParserType enumerators for details on the
     * attributes of each \c ParserType.
     * @param parserType An enumerator specifying the grammar used for parsing
     * @param attribute The instance of the \c ParserAttribute where the AST
     * will be stored.
     * @return \c true if parsing succeeded, \c false otherwise. If parsing
     * fails, a descriptive error message is available using
     * \c LineParser::error()
     */
    template<class ParserAttribute>
    bool parse(ParserType parserType, ParserAttribute& attribute)
    {
        if (data_.empty()) {
            adaptiv::fail("no data to parse");
            return false;
        }

        switch (parserType) {
            case ParserType::List:
            {
                ADAPTIV_ASSERT_STD_VECTOR(
                    ParserAttribute,
                    "the ParserAttribute for a List parser "
                    "must be a std::vector");

                using list_value_type = typename ParserAttribute::value_type;
                auto const listParser =
                    parsers::makeListParser<list_value_type>();

                return doX3Parse(listParser, attribute);
            }
            case ParserType::Array:
            {
                ADAPTIV_ASSERT_STD_VECTOR(
                ParserAttribute,
                "the ParserAttribute for an Array parser "
                "must be a std::vector");

                using array_value_type = typename ParserAttribute::value_type;
                auto const arrayParser =
                    parsers::makeArrayParser<array_value_type>();

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

// extern template for faster build times when using the LineParser -----------

/// Helper alias maker
#define ADAPTIV_MAKE_VECTOR_ALIAS(type, alias)   \
    using alias##_vector_type = std::vector<type>\

/// Helper extern template maker for LineParser::parse
#define ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(AttributeType)                  \
    extern template                                                        \
    bool LineParser::parse(ParserType parserType, AttributeType& attribute)\

/// Helper extern template maker for LineParser::doX3Parse
#define ADAPTIV_MAKE_EXTERN_TEMPLATE_DOX3PARSE(AttributeType)              \
    extern template                                                        \
    bool LineParser::parse(ParserType parserType, AttributeType& attribute)\

// List all parser attributes
ADAPTIV_MAKE_VECTOR_ALIAS(             float, float);
ADAPTIV_MAKE_VECTOR_ALIAS(            double, double);
ADAPTIV_MAKE_VECTOR_ALIAS(             short, short);
ADAPTIV_MAKE_VECTOR_ALIAS(    unsigned short, ushort);
ADAPTIV_MAKE_VECTOR_ALIAS(      unsigned int, uint);
ADAPTIV_MAKE_VECTOR_ALIAS(               int, int);
ADAPTIV_MAKE_VECTOR_ALIAS(              long, long);
ADAPTIV_MAKE_VECTOR_ALIAS(     unsigned long, ulong);
ADAPTIV_MAKE_VECTOR_ALIAS(         long long, longlong);
ADAPTIV_MAKE_VECTOR_ALIAS(unsigned long long, ulonglong);
ADAPTIV_MAKE_VECTOR_ALIAS(       std::string, string);

ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(float_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(double_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(short_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(uint_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(int_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(long_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(ulong_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(ulonglong_vector_type);
ADAPTIV_MAKE_EXTERN_TEMPLATE_PARSE(string_vector_type);



ADAPTIV_INPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_INPUT_HPP
