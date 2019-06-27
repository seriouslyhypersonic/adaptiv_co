#include <iostream>
#include <cstdlib>

#include <vector>

#include <adaptiv/utility/input.hpp>

#include "config.hpp"
#include "parsers.hpp"
#include "solve.hpp"
#include "ping.hpp"

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
    // Check command line arguments
    if (argc != 3) {
        std::cerr <<
                  "  Usage: client <address> <port>\n" <<
                  "Example:\n" <<
                  "         client localhost 8080\n";
        return EXIT_FAILURE;
    }
    auto host = argv[1];
    auto port = argv[2];

    client::welcomeMessage();

    input::LineParser parser(std::cin);

    while (true) {
        parser.getline(">");
        auto command = parser.popFront();

        if (command == "ping") {
            client::ping(host, port);
        } else if (command == "solve") {
            client::solve(host, port);
        } else if (command == "help") {
            client::help();
        }else if (command == "parsers") {
            client::parsers();
        } else if (command == "details") {
            client::details(host, port);
        } else if (command == "exit") {
            break;
        } else {
            std::cerr << "error: invalid command '" << command << "'\n";
        }
    }

    return EXIT_SUCCESS;
}
