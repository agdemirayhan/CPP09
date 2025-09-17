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

// BitcoinExchange::BitcoinExchange(const std::string &csvPath)
// {
// 	loadCsv(csvPath);
// 	if (_rates.empty())
// 		throw std::runtime_error("Error: csv is empty or unreadable.");
// }

// void BitcoinExchange::loadCsv(const std::string &path)
// {
// 		double rate;

// 	std::ifstream in(path.c_str());
// 	if (!in)
// 		throw std::runtime_error("Error: could not open data file.");
// 	std::string line;
// 	// optional header: "date,exchange_rate"
// 	if (std::getline(in, line))
// 	{
// 		if (line.find("date") == std::string::npos
// 			|| line.find("exchange_rate") == std::string::npos)
// 		{
// 			// first line is data, not header
// 			std::string date, rateStr;
// 			std::string left, right;
// 			if (splitKV(line, ',', left, right))
// 			{
// 				date = trim(left);
// 				rateStr = trim(right);
// 				if (!isValidDate(date))
// 					throw std::runtime_error("Error: bad csv date: " + date);
// 				if (!parseDouble(rateStr, rate))
// 					throw std::runtime_error("Error: bad csv rate: " + rateStr);
// 				_rates[date] = rate;
// 			}
// 		}
// 	}
// 	while (std::getline(in, line))
// 	{
// 		if (line.empty())
// 			continue ;
// 		std::string left, right;
// 		if (!splitKV(line, ',', left, right))
// 			throw std::runtime_error("Error: bad csv line: " + line);
// 		std::string date = trim(left);
// 		std::string rateStr = trim(right);
// 		if (!isValidDate(date))
// 			throw std::runtime_error("Error: bad csv date: " + date);
// 		if (!parseDouble(rateStr, rate))
// 			throw std::runtime_error("Error: bad csv rate: " + rateStr);
// 		_rates[date] = rate;
// 	}
// }

// double BitcoinExchange::evaluate(const std::string &date, double value) const
// {
// 	if (!isValidDate(date))
// 		throw std::runtime_error("Error: bad input => " + date);
// 	if (value < 0.0)
// 		throw std::runtime_error("Error: not a positive number.");
// 	if (value > 1000.0) // 42 subject sınırı
// 		throw std::runtime_error("Error: too large a number.");

// 	// map'te arama: date için <= en büyük anahtar
// 	std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);
// 	if (it == _rates.end())
// 	{
// 		// date tüm anahtarlardan büyük -> son elemanı kullan
// 		--it;
// 	}
// 	else if (it->first != date)
// 	{
// 		if (it == _rates.begin())
// 			throw std::runtime_error("Error: no earlier rate for " + date);
// 		--it; // bir önceki <= date
// 	}
// 	return (value * it->second);
// }

// bool BitcoinExchange::isLeap(int y)
// {
// 	return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
// }

// bool BitcoinExchange::isValidDate(const std::string &d)
// {
// 	int	y;
// 	int	m;
// 	int	day;
// 	int	mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// 	int	maxd;

// 	// beklenen: YYYY-MM-DD
// 	if (d.size() != 10 || d[4] != '-' || d[7] != '-')
// 		return (false);
// 	for (size_t i = 0; i < d.size(); ++i)
// 	{
// 		if (i == 4 || i == 7)
// 			continue ;
// 		if (!std::isdigit(d[i]))
// 			return (false);
// 	}
// 	y = std::atoi(d.substr(0, 4).c_str());
// 	m = std::atoi(d.substr(5, 2).c_str());
// 	day = std::atoi(d.substr(8, 2).c_str());
// 	if (y < 2009)
// 		return (false); // bitcoin’in başlangıcına göre tipik sınır
// 	if (m < 1 || m > 12)
// 		return (false);
// 	maxd = mdays[m - 1];
// 	if (m == 2 && isLeap(y))
// 		maxd = 29;
// 	if (day < 1 || day > maxd)
// 		return (false);
// 	return (true);
// }

// bool BitcoinExchange::splitKV(const std::string &line, char sep,
// 	std::string &left, std::string &right)
// {
// 	std::string::size_type p = line.find(sep);
// 	if (p == std::string::npos)
// 		return (false);
// 	left = line.substr(0, p);
// 	right = line.substr(p + 1);
// 	return (true);
// }

// std::string BitcoinExchange::trim(const std::string &s)
// {
// 	std::string::size_type b = 0, e = s.size();
// 	while (b < e && (s[b] == ' ' || s[b] == '\t' || s[b] == '\r'))
// 		++b;
// 	while (e > b && (s[e - 1] == ' ' || s[e - 1] == '\t' || s[e - 1] == '\r'))
// 		--e;
// 	return (s.substr(b, e - b));
// }

// bool BitcoinExchange::parseDouble(const std::string &s, double &out)
// {
// 	double	v;
// 	char	c;

// 	std::istringstream iss(s);
// 	if (!(iss >> v))
// 		return (false);
// 	if (iss >> c)
// 		return (false); // trailing garbage
// 	out = v;
// 	return (true);
// }
