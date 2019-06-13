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
#include <string>
#include <vector>
#include <type_traits>

#include <adaptiv/macros.hpp>
#include <adaptiv/traits/traits.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN

namespace input {

namespace qi = boost::spirit::qi;       // The parser library
namespace ascii = boost::spirit::ascii;
using ascii::space;                     // A space parser

/// Number list grammar: a grammar for comma separated numbers
template<class Iterator>
struct NumberListParser
        : qi::grammar<Iterator, std::vector<double>, ascii::space_type>
{
    NumberListParser(): NumberListParser::base_type(start_)
    {
        using qi::double_;

        start_ = double_ % ',';
    }

    qi::rule<Iterator, std::vector<double>, ascii::space_type> start_;
};

template<class Iterator, class ResultContainer>
bool parseNumberList(Iterator first, Iterator last, ResultContainer& container)
{
    static_assert(
        adaptiv::traits::is_specialization_v<ResultContainer, std::vector>,
        "[adaptiv::utility::parseNumberList] container must be a std::vector");
    static_assert(
        std::is_arithmetic_v<typename ResultContainer::value_type>,
        "[adaptiv::utility::parseNumberList] container elements must be of an "
        "arithmetic type");

    using qi::double_;
    using qi::phrase_parse;

    bool result = phrase_parse(
        first,                  // Start iterator
        last,                   // End iterator
        // Begin grammar
        double_ % ',',
        // End grammar
        space,                  // The skip parser
        container               // The parser's attribute
    );

    if (first != last) { // Parsing failed
        return false;
    }
    return result;
}

template<class Iterator, class ResultContainer>
bool parseNumberArray(Iterator first, Iterator last, ResultContainer& container)
{
    static_assert(
    adaptiv::traits::is_specialization_v<ResultContainer, std::vector>,
    "[adaptiv::utility::parseNumberArray] container must be a std::vector");
    static_assert(
    std::is_arithmetic_v<typename ResultContainer::value_type>,
    "[adaptiv::utility::parseNumberArray] container elements must be of an "
    "arithmetic type");

    using qi::double_;
    using qi::phrase_parse;

    bool result = phrase_parse(
    first,                  // Start iterator
    last,                   // End iterator
    // Begin grammar
    '[' >> *double_ >> ']',
    // End grammar
    space,                  // The skip parser
    container               // The parser's attribute
    );

    if (first != last) { // Parsing failed
        return false;
    }
    return result;
}

/**
 * @enum Describes the grammar used by the parser
 */
enum class ParserType
{
    /**
     * A comma separated list of numbers, each parsed as a \c double
     * @note The container passed to the parser must be a std::vector and its
     * elements must be of an arithmetic type.
     */
    NumberList,

    /**
     * A space separated array of numbers, each parsed as a \c double and
     * enclosed in brackets
     * @note The container passed to the parser must be a std::vector and its
     * elements must be of an arithmetic type.
     */
    NumberArray
};

/**
 * A simple line parser with various grammars
 */
class LineParser
{
    std::istream& in_;
    std::string data_;

public:
    enum class Empty { };

    explicit LineParser(std::istream& in) : in_(in)
    { }

    /**
     * Parses a line using the grammar specified by ParserType
     * @tparam ResultContainer A container type compatible with the \c
     * ParserType. Please check the documentation of \c ParserType enumerators
     * for details on container compatibility.
     * @param parserType
     * @param container
     * @return
     */
    template<class ResultContainer>
    bool parse(ParserType parserType, ResultContainer& container = Empty())
    {
        bool result = false;
        if (data_.empty()) {
            std::cerr << "no data to parse\n";
            return false;
        }

        switch (parserType) {
            case ParserType::NumberList:
                return parseNumberList(data_.begin(), data_.end(), container);
            case ParserType::NumberArray:
                return parseNumberArray(data_.begin(), data_.end(), container);
        }
    }

    /**
     * Returns the first word in the sequence delimited to the right by
     * \c whitespace
     * @param whitespace The characters to be interpreted as space
     * @return The first word in the squence
     */
    std::string firstWord(std::string const& whitespace = " \t")
    {
        return data_.substr(0, data().find_first_of(whitespace));
    }

    /**
     * Checks if the sequence starts with the prefix specified by \c word and
     * delimited to the right by \c whitespace
     * @param word The prefix to test agains
     * @param whitespace The characters to be interpreted as space
     * @return \c true if the prefix is found; otherwise, \c false.
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
     * @param word The prefix to test agains
     * @param whitespace The characters to be interpreted as space
     * @return \c true if the prefix is found; otherwise, \c false.
     */
    bool iStartsWith(
        std::string const& word,
        std::string const& whitespace = " \t")
    {

        return boost::iequals(firstWord(whitespace),  word);
    }

    void popFront()
    {
        boost::erase_first(data_, firstWord());
        boost::trim_left(data_);
    }

    /// Show prompt with and optional message prepended and get the line data
    std::istream& getline(std::string const& prompt = "")
    {
        if (!prompt.empty()) {
            std::cout << prompt << " ";
        }

        std::getline(in_, data_);
        boost::trim_left(data_);

        return in_;
    }

    std::string const& data() const
    {
        return data_;
    }

    void print() const
    {
        std::cout << "data=\"" << data_ << "\"\n";
    }

};

} //namespace input

ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NAMESPACE_END

#endif //ADAPTIV_INPUT_HPP
