#include "../include/BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string trim(const std::string &s) {
    std::string::size_type b = 0, e = s.size();
    while (b < e && (s[b] == ' ' || s[b] == '\t')) ++b;
    while (e > b && (s[e-1] == ' ' || s[e-1] == '\t')) --e;
    return s.substr(b, e - b);
}

// static bool	parseInputLine(const std::string &line, std::string &date,
// 		double &value)
// {
// 	double	v;

// 	std::string l = line;
// 	if (l.empty())
// 		return (false);
// 	if (l.find("date") != std::string::npos
// 		&& l.find("value") != std::string::npos)
// 		return (false);
// 	std::string::size_type bar = l.find('|');
// 	if (bar == std::string::npos)
// 	{
// 		std::cerr << "Error: bad input => " << line << std::endl;
// 		return (false);
// 	}
// 	std::string left = l.substr(0, bar);
// 	std::string right = l.substr(bar + 1);
// 	while (!left.empty() && (left[0] == ' ' || left[0] == '\t'))
// 		left.erase(0, 1);
// 	while (!left.empty() && (left[left.size() - 1] == ' ' || left[left.size()
// 			- 1] == '\t'))
// 		left.erase(left.size() - 1);
// 	while (!right.empty() && (right[0] == ' ' || right[0] == '\t'))
// 		right.erase(0, 1);
// 	while (!right.empty() && (right[right.size() - 1] == ' '
// 			|| right[right.size() - 1] == '\t'))
// 		right.erase(right.size() - 1);
// 	date = left;
// 	if (!BitcoinExchange::parseDouble(right, v))
// 	{
// 		std::cerr << "Error: bad value => " << right << std::endl;
// 		return (false);
// 	}
// 	value = v;
// 	return (true);
// }

int	main(int argc, char **argv)
{
	double	value;
	double	result;
	bool	firstLine;

	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try
	{
		BitcoinExchange be("data/data.csv");
		// // TEST: _dataMap içeriğini yazdır
		// std::cout << "---- CSV'den okunan veriler ----" << std::endl;
		// for (std::map<std::string,
		// 	float>::const_iterator it = be.getDataMap().begin(); it != be.getDataMap().end(); ++it)
		// {
		// 	std::cout << it->first << " => " << it->second << std::endl;
		// }
		// std::cout << "--------------------------------" << std::endl;
		 std::ifstream in(argv[1]);
        if (!in) {
            std::cerr << "Error: could not open file." << std::endl;
            return 1;
        }

        std::string line;
        bool firstLine = true;

        while (std::getline(in, line)) {
            if (line.empty()) continue;

            // header satırını atla
            if (firstLine && line.find("date") != std::string::npos) {
                firstLine = false;
                continue;
            }
            firstLine = false;

            std::size_t bar = line.find('|');
            if (bar == std::string::npos) {
                std::cerr << "Error: bad input => " << line << std::endl;
                continue;
            }

            std::string date = trim(line.substr(0, bar));
			if (!BitcoinExchange::isValidDate(date)) 
			{
				std::cerr << "Error: bad input => " << date << std::endl;
				continue;
			}
            std::string valueStr = trim(line.substr(bar + 1));

			// valueStr'yi double'a çevir
			std::istringstream iss(valueStr);
			double value;
			if (!(iss >> value)) {
				std::cerr << "Error: bad value => " << valueStr << std::endl;
				continue;
			}
			if (value < 0) {
				std::cerr << "Error: not a positive number." << std::endl;
				continue;
			}
			if (value > 1000) {
				std::cerr << "Error: too large a number." << std::endl;
				continue;
			}
			std::cout << "Date: [" << date << "], Value: " << value << std::endl;
        }
		return (0);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
}
