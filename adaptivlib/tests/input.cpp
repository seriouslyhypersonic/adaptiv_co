#include <iostream>
#include <string>
#include <cstddef>
#include <limits>

#include <adaptiv/utility/input.hpp>
#include <adaptiv/math/random.hpp>

#include <tests/testing.hpp>
#include <tests/input.hpp>

namespace input = adaptiv::utility::input;

// Test settings
int const size         =  100;
long const intMin      = -1'000'000'000'000;
long const intMax      =  1'000'000'000'000;
double const doubleMin = -1e12;
double const doubleMax =  1e12;
double const absError  =  0.001;


#define STRING_VECTOR                                         \
std::vector<std::string>{                                     \
    "Hello, world!",                                          \
    "Supercalifragilisticexpialidocious",                     \
    "A long time ago in a galaxy far, far away...",           \
    "Lorem ipsum dolor sit amet, et ac aenean, enim integer.",\
    "Eu cupidatat nisl, integer non enim.",                   \
    "Ut ligula, at adipisci.",                                \
    "Sed purus, lorem varius."                                \
}                                                             \

using iterator_type = std::string::iterator;

/// Test the list grammar
TEST(Input, ListParser)
{
    bool noFailure = false; // Check parser components failure

    // Parse unsigned integers -------------------------------------------------
    auto ulongs = randomVector<unsigned long>(size, 0, intMax);
    auto ulongList = makeList(ulongs, ", ");

    // Create a parser for a comma separated list of ulongs
    input::ListParser<unsigned long, iterator_type> ulongListParser;

    // Parse
    std::vector<unsigned long> ulongsParsed; // For synthesized attributes
    ulongsParsed.reserve(size);              // Performance optimization
    noFailure = input::qi::phrase_parse(
        ulongList.begin(),
        ulongList.end(),
        ulongListParser,
        input::ascii::space,
        ulongsParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_EQ(ulongsParsed, ulongs);

    // Parse integers ----------------------------------------------------------
    auto longs =
        randomVector<long>(size, intMin, intMax);
    auto longList = makeList(longs, ", ");

    // Create a parser for a comma separated list of longs
    input::ListParser<long, iterator_type> longListParser;

    // Parse
    std::vector<long> longsParsed;
    longsParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        longList.begin(),
        longList.end(),
        longListParser,
        input::ascii::space,
        longsParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_EQ(longsParsed, longs);

    // Parse doubles -----------------------------------------------------------
    auto doubles = randomVector<double>(size, doubleMin, doubleMax);
    auto listWithSpaces = makeList(doubles, ", ");
    auto listWithTabs = makeList(doubles, "\t,\t");

    // Create a parser for a comma separated list of doubles
    input::ListParser<double, iterator_type> doubleListParser;

    // Parse the list with spaces
    std::vector<double> doublesParsed;
    doublesParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        listWithSpaces.begin(),
        listWithSpaces.end(),
        doubleListParser,
        input::ascii::space,
        doublesParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_NEAR(doublesParsed, doubles, absError);

    // Parse the list with tabs
    doublesParsed.clear();
    doublesParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        listWithTabs.begin(),
        listWithTabs.end(),
        doubleListParser,
        input::ascii::space,
        doublesParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_NEAR(doublesParsed, doubles, absError);

    // Parse strings -----------------------------------------------------------
    auto strings = STRING_VECTOR;
    auto stringList = makeList(strings, ", ");

    // Create a parser for a list of strings
    input::ListParser<std::string, iterator_type> stringListParser;

    std::vector<std::string> stringsParsed;
    stringsParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        stringList.begin(),
        stringList.end(),
        stringListParser,
        input::ascii::space,
        stringsParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_EQ(stringsParsed, strings);
}

/// Test the array grammar
TEST(Input, ArrayParser)
{
    bool noFailure = false; // Check parser components failure

    // Parse unsigned integers -------------------------------------------------
    auto ulongs = randomVector<unsigned long>(size, 0, intMax);
    auto ulongArray = makeArray(ulongs);

    // Create a parser for an array of unsigned integers
    input::ArrayParser<unsigned long, iterator_type> ulongArrayParser;

    // Parse array
    std::vector<unsigned long> ulongsParsed;
    ulongsParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        ulongArray.begin(),
        ulongArray.end(),
        ulongArrayParser,
        input::ascii::space,
        ulongsParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_EQ(ulongsParsed, ulongs);

    // Parse integers ----------------------------------------------------------
    auto longs = randomVector<long>(size, intMin, intMax);
    auto longArray = makeArray(longs);

    // Create a parser for an array of integers
    input::ArrayParser<long, iterator_type> longArrayParser;

    // Parse array
    std::vector<long> longsParsed;
    longsParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        longArray.begin(),
        longArray.end(),
        longArrayParser,
        input::ascii::space,
        longsParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_EQ(longsParsed, longs);

    // Parse doubles -----------------------------------------------------------
    auto doubles = randomVector<double>(size, 0, 1e12);
    auto doubleArray = makeArray(doubles);

    // Create a parser for an array of doubles
    input::ArrayParser<double, iterator_type> doubleArrayParser;

    // Parse array of numbers
    std::vector<double> doublesParsed;
    doublesParsed.reserve(size);
    noFailure = input::qi::phrase_parse(
        doubleArray.begin(),
        doubleArray.end(),
        doubleArrayParser,
        input::ascii::space,
        doublesParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_NEAR(doublesParsed, doubles, absError);

    // Parse strings -----------------------------------------------------------
    auto strings = STRING_VECTOR;
    auto stringArray = makeArray(strings);

    // Create a parser for an array of strings
    input::ArrayParser<std::string, iterator_type> stringArrayParser;

    // Parse array of strings
    std::vector<std::string> stringsParsed;
    noFailure = input::qi::phrase_parse(
        stringArray.begin(),
        stringArray.end(),
        stringArrayParser,
        input::ascii::space,
        stringsParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_EQ(stringsParsed, strings);
}