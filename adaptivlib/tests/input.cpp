#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>
#include <cstddef>

#include <adaptiv/utility/input.hpp>
#include <adaptiv/math/random.hpp>

#include <gtest/gtest.h>

namespace input = adaptiv::utility::input;
namespace math = adaptiv::math;

std::string makeList(
        std::vector<double> const& numbers,
        std::string const& separator)
{
    std::string list;
    for (int i = 0; i < numbers.size(); ++i) {
        list.append(std::to_string(numbers[i]));
        if (i != numbers.size() - 1) {
            list.append(separator);
        }
    }
    return list;
}

TEST(Input, NumberListParser)
{
    int const length = 1000;
    double const absError = 0.00001;

    // Fill a vector with random numbers
    std::vector<double> numbers(length);
    std::generate(numbers.begin(), numbers.end(),
            [](){
                    return math::random<double>(0, 1);
                });

    auto listWithSpaces = makeList(numbers, ", ");
    auto listWithTabs = makeList(numbers, "\t,\t");

    // Create a parser for a comma separated list of numbers
    using iterator_type = std::string::iterator;
    input::NumberListParser<iterator_type> numberListParser;

    // Parse the list with spaces
    std::vector<double> parsedNumbers;
    parsedNumbers.reserve(length);
    input::qi::phrase_parse(
        listWithSpaces.begin(),
        listWithSpaces.end(),
        numberListParser,
        input::ascii::space,
        parsedNumbers);

    for (int i = 0; i < parsedNumbers.size(); ++i) {
        ASSERT_NEAR(parsedNumbers[i], numbers[i], absError);
    }

    // Parse the list with tabs
    parsedNumbers.clear();
    parsedNumbers.reserve(500);
    input::qi::phrase_parse(
        listWithSpaces.begin(),
        listWithSpaces.end(),
        numberListParser,
        input::ascii::space,
        parsedNumbers);

    for (int i = 0; i < parsedNumbers.size(); ++i) {
        ASSERT_NEAR(parsedNumbers[i], numbers[i], absError);
    }
}