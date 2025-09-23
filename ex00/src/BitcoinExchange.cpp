#include "BitcoinExchange.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

// ---------- small helpers ----------
static std::string trim(const std::string& s) {
    std::string::size_type b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

static bool safeParseFloat(const std::string& s, float& out) {
    try {
        // std::stof handles leading/trailing spaces; we trim anyway for clarity
        std::size_t idx = 0;
        float v = std::stof(s, &idx);
        // allow only trailing spaces after the parsed number
        for (; idx < s.size(); ++idx) {
            if (!std::isspace(static_cast<unsigned char>(s[idx]))) return false;
        }
        out = v;
        return true;
    } catch (...) {
        return false;
    }
}

// ---------- public API ----------
const std::string& BitcoinExchange::getCsvPath() const {
    return _csvPath;
}

BitcoinExchange::BitcoinExchange(const std::string& csvPath)
: _csvPath(csvPath) // keep a copy if you expose getCsvPath()
{
    loadCsv(csvPath);
}

// Load the historical BTC rates from a CSV file (format: "date,rate").
// Skips a header line if it contains "date".
void BitcoinExchange::loadCsv(const std::string& csvPath)
{
    std::ifstream file(csvPath.c_str());
    if (!file) {
        throw std::runtime_error("Error: could not open data file.");
    }

    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Skip header if present
        if (firstLine && line.find("date") != std::string::npos) {
            firstLine = false;
            continue;
        }
        firstLine = false;

        // Split on ','
        std::size_t comma = line.find(',');
        if (comma == std::string::npos) {
            // Malformed line: no comma. Skip silently or log if you prefer.
            continue;
        }

        std::string key      = trim(line.substr(0, comma));       // date string
        std::string valueStr = trim(line.substr(comma + 1));      // rate string

        // Optional: validate date format early (prevents bad keys in the map)
        if (!isValidDate(key)) {
            // If you prefer to be silent, replace with "continue;"
            std::cerr << "Warning: invalid date in CSV => " << key << std::endl;
            continue;
        }

        float value = 0.0f;
        if (!safeParseFloat(valueStr, value)) {
            std::cerr << "Error: invalid rate in CSV => " << valueStr << std::endl;
            continue;
        }

        _dataMap[key] = value;
    }
}

// Leap year check (Gregorian rule)
bool BitcoinExchange::isLeap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Validate "YYYY-MM-DD" and that day fits the month (with leap-year Feb)
bool BitcoinExchange::isValidDate(const std::string& d)
{
    // Basic shape: YYYY-MM-DD
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (std::size_t i = 0; i < d.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(d[i]))) return false;
    }

    int y   = std::atoi(d.substr(0, 4).c_str());
    int m   = std::atoi(d.substr(5, 2).c_str());
    int day = std::atoi(d.substr(8, 2).c_str());

    // Reasonable lower bound for this project
    if (y < 2009) return false;
    if (m < 1 || m > 12) return false;

    int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = mdays[m - 1];
    if (m == 2 && isLeap(y)) maxd = 29;

    if (day < 1 || day > maxd) return false;
    return true;
}
