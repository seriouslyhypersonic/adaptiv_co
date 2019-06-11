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

    parser.getline("numbers?\n>");
    if (!parser.startsWith("numbers")) return EXIT_FAILURE;
    parser.popFront();

    std::vector<double> numbers;
    parser.parse(ParserType::NumberList, numbers);
    print(numbers);

    parser.getline("array?\n>");
    if (!parser.startsWith("array")) return EXIT_FAILURE;
    parser.popFront();

    parser.parse(ParserType::NumberArray, numbers);
    print(numbers);

    return EXIT_SUCCESS;
}