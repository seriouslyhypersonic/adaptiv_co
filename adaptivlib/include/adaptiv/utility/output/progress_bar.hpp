/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP

#include <iostream>
#include <cstddef>
#include <string>

#include <adaptiv/macros.hpp>
#include <adaptiv/utility/output/styles.hpp>
#include <adaptiv/utility/output/platform_console.hpp>

ADAPTIV_NAMESPACE_BEGIN
ADAPTIV_UTILITY_NAMESPACE_BEGIN
ADAPTIV_OUTPUT_NAMESPACE_BEGIN

class ProgressBar
{
    char elapsedSymbol_;
    char remainingSymbol_;
    double progress_;

    // Terminal dimensions
    std::size_t consoleWidth_;
    std::size_t oldConsoleWidth_;

    static std::size_t const legendLength_;

    std::size_t consoleWidth()
    {
        return platform::consoleWidth();
    }

    std::size_t barWidth() { return consoleWidth_ - legendLength_; }

public:
    explicit ProgressBar(
        char elapsedSymbol = '|',
        char remainingSymbol = ' ',
        double progress = 0)
        : elapsedSymbol_(elapsedSymbol)
        , remainingSymbol_(remainingSymbol)
        , progress_(progress)
        , consoleWidth_(consoleWidth())
        , oldConsoleWidth_(consoleWidth_)
    { }

    void clear()
    {
        std::cout << platform::csi << 's'; // Save the current
        std::cout << std::string(oldConsoleWidth_, ' ');
        std::cout << platform::csi << 'u'; // Restore the cursor position
        std::cout.flush();
    }

    static void error(int percentage)
    {
        using output::style::message;
        using output::style::error;
        using output::style::none;
        std::cout <<
            error << "[--- error ---] " << none << "progress " <<
            message << percentage << '%' << none <<  '\r';
        std::cout.flush();
    }

    void update(double progress)
    {
        progress_ = progress;
        auto percentage = static_cast<int>(progress*100);

        clear();
        if (percentage < 0 || percentage > 100) {
            return error(percentage);
        }

        // Update console and bar dimensions (detect possible resizing)
        oldConsoleWidth_ = consoleWidth_;
        consoleWidth_ = consoleWidth();

        if (!consoleWidth_) {
            return error(percentage);
        }

        std::string barContent(barWidth(), remainingSymbol_);

        auto nElapsedSymbols = static_cast<std::size_t>(barWidth() * progress);
        barContent.replace(
            barContent.begin(),
            barContent.begin() + nElapsedSymbols,
            nElapsedSymbols, elapsedSymbol_);

        // Display bar
        using output::style::none;
        using output::style::message;
        std::cout << '[' << barContent << "] " <<
            message << percentage << "%" << none;
        std::cout << platform::csi << 'u'; // Restore cursor position
        std::cout.flush();
    }
};

inline std::size_t const ProgressBar::legendLength_ =
    sizeof("[] XXX%") - 1; // Do not count null-terminator

ADAPTIV_OUTPUT_NAMESPACE_END
ADAPTIV_UTILITY_NAMESPACE_END
ADAPTIV_NET_NAMESPACE_END

#endif //PROGRESS_BAR_HPP
