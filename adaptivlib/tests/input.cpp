#include <iostream>
#include <string>
#include <cstddef>
#include <limits>
#include <type_traits>

#include <adaptiv/utility/input.hpp>
#include <adaptiv/math/random.hpp>

#include <tests/testing.hpp>
#include <tests/input.hpp>

namespace input = adaptiv::utility::input;

// Test settings
int                const size         =  100;
float              const floatMax     = 10000;
double             const doubleMax    =  1e12;
short              const shortMax     = std::numeric_limits<short>::max();
unsigned short     const ushortMax    = std::numeric_limits<short>::max();
int                const intMax       = std::numeric_limits<int>::max();
unsigned int       const uintMax      = std::numeric_limits<int>::max();
long               const longMax      = std::numeric_limits<long>::max();
unsigned long      const ulongMax     = std::numeric_limits<long>::max();
long long          const longlongMax  = std::numeric_limits<long long>::max();
unsigned long long const ulonglongMax = std::numeric_limits<unsigned long long>::max();
float              const absError     = 0.001;

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

#define ADAPTIV_MAKE_LIST_TEST(type, alias)                              \
    bool alias##IsParsed = false;                                         \
    auto alias##s =                                                      \
        randomVector<type>(size, infimum(alias##Max), alias##Max);       \
    auto alias##List = makeList(alias##s, ", ");                         \
    auto const alias##Parser =                                           \
        input::experimental::parsers::makeListParser<type, false>();     \
    std::vector<type> alias##sParsed;                                    \
    alias##sParsed.reserve(size);                                        \
    alias##IsParsed = boost::spirit::x3::phrase_parse(                   \
        alias##List.begin(),                                             \
        alias##List.end(),                                               \
        alias##Parser,                                                   \
        boost::spirit::x3::ascii::space,                                 \
        alias##sParsed);                                                 \
    ASSERT_TRUE(alias##IsParsed);                                        \
    if constexpr (std::is_floating_point_v<type>) {                      \
        ADAPTIV_ASSERT_ELEMENTS_NEAR(alias##sParsed, alias##s, absError);\
    } else {                                                             \
        ADAPTIV_ASSERT_ELEMENTS_EQ(alias##sParsed, alias##s);            \
    } do { break; } while (true)                                         \
                                                                         \

/// Test the list grammar and all numeric x3_parsers
TEST(Input, X3ListParser)
{
    ADAPTIV_MAKE_LIST_TEST(             float, float);
    ADAPTIV_MAKE_LIST_TEST(            double, double);
    ADAPTIV_MAKE_LIST_TEST(             short, short);
    ADAPTIV_MAKE_LIST_TEST(    unsigned short, ushort);
    ADAPTIV_MAKE_LIST_TEST(      unsigned int, uint);
    ADAPTIV_MAKE_LIST_TEST(               int, int);
    ADAPTIV_MAKE_LIST_TEST(              long, long);
    ADAPTIV_MAKE_LIST_TEST(     unsigned long, ulong);
    ADAPTIV_MAKE_LIST_TEST(         long long, longlong);
    ADAPTIV_MAKE_LIST_TEST(unsigned long long, ulonglong);
}

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
        randomVector<long>(size, infimum(intMax), intMax);
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
    auto doubles = randomVector<double>(size, infimum(doubleMax), doubleMax);
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
    auto longs = randomVector<long>(size, infimum(intMax), intMax);
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
    auto doubles = randomVector<double>(size, infimum(doubleMax), doubleMax);
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

// X3
TEST(Input, X3)
{
    namespace x3 = boost::spirit::x3;
    using x3::ascii::space;

    // Parse doubles
    auto doubles = randomVector<double>(size, infimum(doubleMax), doubleMax);
    auto doubleList = makeList(doubles);

//    std::string doubleList = "1, 2,";

    iterator_type begin = doubleList.begin();
    iterator_type end = doubleList.end();

    // with directive to inject a reference to std::cerr in the error handler
    using boost::spirit::x3::with;

    // Tag to access get a reference to the actual x3::error_handler
    using boost::spirit::x3::error_handler_tag;

    using error_handler_type = boost::spirit::x3::error_handler<iterator_type>;

    // The error handler
    error_handler_type errorHandler(begin, end, std::cerr);

    // The parser
    auto const parser =
    // we pass our error handler to the parser so we can access
    // it later in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(errorHandler))
            [
                input::experimental::parsers::makeListParser<double>()
            ];

    std::vector<double> doublesParsed;
    bool noFailure =
        x3::phrase_parse(
            doubleList.begin(),
            doubleList.end(),
            parser,
            space,
            doublesParsed);

    ASSERT_TRUE(noFailure);
    ADAPTIV_ASSERT_ELEMENTS_NEAR(doublesParsed, doubles, absError);
}