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

#define DUMMY_STRING_VECTOR                                   \
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

/// A test for the the list grammar (including missplaced tabs)
#define ADAPTIV_MAKE_LIST_TEST(type, alias)                              \
    bool alias##IsParsed = false;                                        \
    auto alias##s =                                                      \
        randomVector<type>(size, infimum(alias##Max), alias##Max);       \
    auto alias##List = makeList(alias##s, ",\t");                        \
    auto const alias##Parser =                                           \
        input::parsers::makeListParser<type, false>();                   \
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

/// Test the list grammar and all numeric parsers
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

/// Test the array grammar and the quoted string parser
TEST(Input, X3ArrayParser)
{
    bool isParsed = false;
    std::vector<std::string> strings = {
        "Hello, world!",
        "Supercalifragilisticexpialidocious",
        "A long time ago in a galaxy far, far away...",
        "Lorem ipsum dolor sit amet, et ac aenean, enim integer.",
        "Eu cupidatat nisl, integer non enim.",
        "Ut ligula, at adipisci.",
        "Sed purus, lorem varius."};

    auto stringArray = makeArray(strings);

    // Create a parser for an array of strings
    auto arrayParser = input::parsers::makeArrayParser<std::string, false>();

    // Parse the strings
    std::vector<std::string> stringsParsed;
    isParsed = boost::spirit::x3::phrase_parse(
        stringArray.begin(),
        stringArray.end(),
        arrayParser,
        boost::spirit::x3::ascii::space,
        stringsParsed);

    ASSERT_TRUE(isParsed);
    ADAPTIV_ASSERT_ELEMENTS_EQ(stringsParsed, strings);
}

// X3
TEST(Input, X3s)
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
                input::parsers::makeListParser<double>()
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