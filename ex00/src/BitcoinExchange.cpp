#include "../include/BitcoinExchange.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

const std::string &BitcoinExchange::getCsvPath() const
{
	return (_csvPath);
}

BitcoinExchange::BitcoinExchange(const std::string &csvPath)
{
	loadCsv(csvPath);
}

void BitcoinExchange::loadCsv(const std::string &csvPath)
{
	bool	firstLine;
	float	value;

	std::ifstream file(csvPath);
	std::string line;
	firstLine = true;
	while (std::getline(file, line))
	{
		// Skip header if present
		if (firstLine && line.find("date") != std::string::npos)
		{
			firstLine = false;
			continue ;
		}
		firstLine = false;
		std::size_t delimiter = line.find(",");
		if (delimiter != std::string::npos)
		{
			std::string key = line.substr(0, delimiter);
			std::string valueStr = line.substr(delimiter + 1);
			// trim spaces
			while (!valueStr.empty() && std::isspace(valueStr.front()))
				valueStr.erase(0, 1);
			while (!valueStr.empty() && std::isspace(valueStr.back()))
				valueStr.pop_back();
			try
			{
				value = std::stof(valueStr);
				_dataMap[key] = value;
			}
			catch (const std::exception &e)
			{
				std::cerr << "Error: invalid rate in CSV => " << valueStr << std::endl;
			}
		}
	}
}

bool BitcoinExchange::isLeap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool BitcoinExchange::isValidDate(const std::string &d)
{
    // YYYY-MM-DD formatı
    if (d.size() != 10 || d[4] != '-' || d[7] != '-')
        return false;
    for (size_t i = 0; i < d.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(d[i])))
            return false;
    }

    int y = std::atoi(d.substr(0, 4).c_str());
    int m = std::atoi(d.substr(5, 2).c_str());
    int day = std::atoi(d.substr(8, 2).c_str());

    if (y < 2009)            // proje için makul alt sınır
        return false;
    if (m < 1 || m > 12)
        return false;

    int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = mdays[m - 1];
    if (m == 2 && isLeap(y))
        maxd = 29;

    if (day < 1 || day > maxd)
        return false;

    return true;
}
