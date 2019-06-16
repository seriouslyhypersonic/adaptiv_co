#include <iostream>
#include <cstdlib>

#include <vector>

#include <adaptiv/utility/input.hpp>

#include <adaptiv/traits/traits.hpp>

namespace traits = adaptiv::traits;
namespace input = adaptiv::utility::input;
using input::ParserType;

template<class T>
void print(T const& t) { std::cout << t << '\n'; }

template<class T>
void print(std::vector<T> const& vec)
{
    for (auto const& item : vec)
        std::cout << '[' << item << "] ";
    std::cout << '\n';
}

int main(int argc, char** argv)
{
//    // Check command line arguments
//    if (argc != 3) {
//        std::cerr <<
//                  "  Usage: client <address> <port>\n" <<
//                  "Example:\n" <<
//                  "         client localhost 8080\n";
//        return EXIT_FAILURE;
//    }
//    auto host = argv[1];
//    auto port = argv[2];

    input::LineParser parser(std::cin);

//    parser.getline("list?\n>");
//    if (!parser.startsWith("list")) return EXIT_FAILURE;
//    parser.popFront();
//
//    std::vector<int> list;
//    bool success = parser.parse(ParserType::List, list);
//    if (!success) {
//        std::cerr << parser.error() << '\n';
//        return EXIT_FAILURE;
//    }
//    print(list);

    parser.getline("array?\n>");
    std::vector<std::string> array;
    if (!parser.startsWith("array")) return EXIT_FAILURE;
    parser.popFront();

    if(!parser.parse(ParserType::Array, array)) {
        std::cerr << parser.error() << '\n';
    }
    print(array);

    return EXIT_SUCCESS;
}