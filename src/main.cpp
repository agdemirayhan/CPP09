#include "../include/BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <string>

static bool	parseInputLine(const std::string &line, std::string &date,
		double &value)
{
	double	v;

	// beklenen: "YYYY-MM-DD | value"
	std::string l = line;
	// boş satırları atla
	if (l.empty())
		return (false);
	// Header satırını tolere et (subject genelde "date | value")
	if (l.find("date") != std::string::npos
		&& l.find("value") != std::string::npos)
		return (false);
	std::string::size_type bar = l.find('|');
	if (bar == std::string::npos)
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return (false);
	}
	std::string left = l.substr(0, bar);
	std::string right = l.substr(bar + 1);
	// trim
	while (!left.empty() && (left[0] == ' ' || left[0] == '\t'))
		left.erase(0, 1);
	while (!left.empty() && (left[left.size() - 1] == ' ' || left[left.size()
			- 1] == '\t'))
		left.erase(left.size() - 1);
	while (!right.empty() && (right[0] == ' ' || right[0] == '\t'))
		right.erase(0, 1);
	while (!right.empty() && (right[right.size() - 1] == ' '
			|| right[right.size() - 1] == '\t'))
		right.erase(right.size() - 1);
	date = left;
	if (!BitcoinExchange::parseDouble(right, v))
	{
		std::cerr << "Error: bad value => " << right << std::endl;
		return (false);
	}
	value = v;
	return (true);
}

int	main(int argc, char **argv)
{
			double value;
	double	result;

	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return (1);
	}
	try
	{
		// data.csv proje kökünde bekleniyor
		BitcoinExchange be("data.csv");
		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cerr << "Error: could not open file." << std::endl;
			return (1);
		}
		std::string line;
		while (std::getline(in, line))
		{
			std::string date;
			if (!parseInputLine(line, date, value))
				continue ;
			try
			{
				result = be.evaluate(date, value);
				std::cout << date << " => " << value << " = " << result << std::endl;
			}
			catch (const std::exception &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
