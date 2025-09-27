#include "../include/BitcoinExchange.hpp"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

// ---------- trimming helpers ----------
static std::string_view ltrim(std::string_view sv) {
    auto it = std::find_if_not(sv.begin(), sv.end(),
        [](unsigned char c){ return std::isspace(c); });
    return std::string_view(&*it, sv.end() - it);
}
static std::string_view rtrim(std::string_view sv) {
    auto it = std::find_if_not(sv.rbegin(), sv.rend(),
        [](unsigned char c){ return std::isspace(c); }).base();
    return std::string_view(sv.data(), it - sv.begin());
}
static std::string_view trim_view(std::string_view sv) {
    return rtrim(ltrim(sv));
}
static std::string trim_copy(std::string_view sv) {
    std::string_view tv = trim_view(sv);
    return std::string(tv.data(), tv.size());
}

// ---------- small helpers ----------
static bool isHeaderLine(const std::string& line) {
    return line.find("date") != std::string::npos && line.find("value") != std::string::npos;
}

static bool splitInputLine(const std::string& line, std::string& dateOut, std::string& valueOut) {
    std::size_t bar = line.find('|');
    if (bar == std::string::npos) return false;
    dateOut  = trim_copy(std::string_view(line).substr(0, bar));
    valueOut = trim_copy(std::string_view(line).substr(bar + 1));
    return true;
}

static bool parseValue(const std::string& valueStr, double& valueOut) {
    try {
        std::size_t idx = 0;
        double v = std::stod(valueStr, &idx);
        // allow trailing spaces only
        for (; idx < valueStr.size(); ++idx) {
            if (!std::isspace(static_cast<unsigned char>(valueStr[idx])))
                return false;
        }
        valueOut = v;
        return true;
    } catch (...) {
        return false;
    }
}

static bool validateValue(double v) {
    if (v < 0) {
        std::cerr << RED <<"Error: not a positive number." << RESET << std::endl;
        return false;
    }
    if (v > 1000) {
        std::cerr << RED << "Error: too large a number." << RESET << std::endl;
        return false;
    }
    return true;
}

static bool findRateForDate(const std::map<std::string,float>& rates,
                            const std::string& date, float& rateOut)
{
    if (rates.empty()) {
        std::cerr << RED <<"Error: csv is empty or unreadable." << RESET << std::endl;
        return false;
    }

    auto it = rates.lower_bound(date);
    if (it == rates.end()) {
        // date is greater than all keys -> use last record
        --it;
    } else if (it->first != date) {
        // not an exact match -> use previous (<= date)
        if (it == rates.begin()) {
            std::cerr << RED << "Error: no earlier rate for " << date << RESET << std::endl;
            return false;
        }
        --it;
    }
    rateOut = it->second;
    return true;
}

static void processInputStream(BitcoinExchange& be, std::istream& in)
{
    std::cout << std::fixed << std::setprecision(2);

    std::string line;
    bool firstLine = true;

    const std::map<std::string,float>& rates = be.getDataMap();

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        // skip header once
        if (firstLine && isHeaderLine(line)) {
            firstLine = false;
            continue;
        }
        firstLine = false;

        std::string date, valueStr;
        if (!splitInputLine(line, date, valueStr)) {
            std::cerr << RED << "Error: bad input => " << line << RESET << std::endl;
            continue;
        }

        if (!BitcoinExchange::isValidDate(date)) {
            std::cerr << RED << "Error: bad input => " << date << RESET << std::endl;
            continue;
        }

        double value = 0.0;
        if (!parseValue(valueStr, value)) {
            std::cerr << RED << "Error: bad value => " << valueStr << RESET << std::endl;
            continue;
        }
        if (!validateValue(value)) continue;    

        float rate = 0.0f;
        if (!findRateForDate(rates, date, rate)) continue;

        double result = value * rate;
        std::cout << GREEN << date << " => " << value << " = " << result << RESET << std::endl;
    }
}

// ---------- main ----------
int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << RED << "Error: could not open file." << RESET << std::endl;
        return 1;
    }

    try {
        BitcoinExchange be("data.csv");

        std::ifstream in(argv[1]);
        if (!in) {
            std::cerr << "Error: could not open file." << RESET << std::endl;
            return 1;
        }

        processInputStream(be, in);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
