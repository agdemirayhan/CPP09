#include "BitcoinExchange.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

BitcoinExchange::BitcoinExchange(const std::string &csvPath)
{
	loadCsv(csvPath);
	if (_rates.empty())
		throw std::runtime_error("Error: csv is empty or unreadable.");
}

void BitcoinExchange::loadCsv(const std::string &path)
{
	double rate;

	std::ifstream in(path.c_str());
	if (!in)
		throw std::runtime_error("Error: could not open data file.");
	std::string line;
	// optional header: "date,exchange_rate"
	if (std::getline(in, line))
	{
		if (line.find("date") == std::string::npos
			|| line.find("exchange_rate") == std::string::npos)
		{
			// first line is data, not header
			std::string date, rateStr;
			std::string left, right;
			// if (splitKV(line, ',', left, right))
			// {
			// 	date = trim(left);
			// 	rateStr = trim(right);
			// 	if (!isValidDate(date))
			// 		throw std::runtime_error("Error: bad csv date: " + date);
			// 	if (!parseDouble(rateStr, rate))
			// 		throw std::runtime_error("Error: bad csv rate: " + rateStr);
			// 	_rates[date] = rate;
			// }
		}
	}
	// while (std::getline(in, line))
	// {
	// 	if (line.empty())
	// 		continue ;
	// 	std::string left, right;
	// 	if (!splitKV(line, ',', left, right))
	// 		throw std::runtime_error("Error: bad csv line: " + line);
	// 	std::string date = trim(left);
	// 	std::string rateStr = trim(right);
	// 	if (!isValidDate(date))
	// 		throw std::runtime_error("Error: bad csv date: " + date);
	// 	if (!parseDouble(rateStr, rate))
	// 		throw std::runtime_error("Error: bad csv rate: " + rateStr);
	// 	_rates[date] = rate;
	// }
}
